#include "DriveBase.h"
#include "XeroRobotBase.h"
#include "RobotState.h"
#include "ParamsParser.h"
#include "RobotStateEstimator.h"
#include "Kinematics.h"
#include "ModuleDefintions.h"
#include <cassert>
#include <iostream>
#include <cmath>
#include <numeric>

using namespace xero::math;
using namespace xero::motion;
using namespace xero::pathfinder;

namespace xerolib
{
	DriveBase::DriveBase(XeroRobotBase &robot,
						 std::shared_ptr<frc::SpeedController> left_motor_p, std::shared_ptr<frc::SpeedController> right_motor_p,
						 std::shared_ptr<frc::Encoder> left_enc_p,  std::shared_ptr<frc::Encoder> right_enc_p,
						 std::shared_ptr<AHRS> navx_p) : DriveBase(robot)
	{
		setMotors(left_motor_p, right_motor_p) ;
		setEncoders(left_enc_p, right_enc_p) ;
		setNavX(navx_p) ;
	}
	
	DriveBase::DriveBase(XeroRobotBase &robot,
						 std::list<std::shared_ptr<frc::SpeedController>> left_motors, std::list<std::shared_ptr<frc::SpeedController>> right_motors,
						 std::shared_ptr<frc::Encoder> left_enc_p,  std::shared_ptr<frc::Encoder> right_enc_p,
						 std::shared_ptr<AHRS> navx_p) : DriveBase(robot)
	{
		setMotors(left_motors, right_motors) ;
		setEncoders(left_enc_p, right_enc_p) ;
		setNavX(navx_p) ;
	}
	
	DriveBase::DriveBase(XeroRobotBase &robot) : SubsystemBase("drivebase", robot), m_left_speed(4), m_right_speed(4)
	{
		m_mode = Mode::Idle;

		m_left_voltage = 0.0;
		m_right_voltage = 0.0;
		m_left_last_voltage = std::nan("");
		m_right_last_voltage = std::nan("");

		initPIDConstants();
		m_yaw = 0.0;
	}

	DriveBase::~DriveBase()
	{
		m_running = false;
		m_velocity_thread.join();
		m_drivebase_control_thread.join();
		m_estimator_thread.join();
	}

	void DriveBase::startThreads()
	{
		if (m_left_motors.size() > 0 && m_right_motors.size() > 0 && m_left_encoder_p != nullptr && m_right_encoder_p != nullptr && m_navx_p != nullptr)
		{
			m_running = true;
			m_velocity_thread = std::thread(&DriveBase::velocityThread, this);
			m_drivebase_control_thread = std::thread(&DriveBase::controlThread, this);
			m_estimator_thread = std::thread(&DriveBase::stateThread, this);
		}
	}

	void DriveBase::stateThread()
	{
		std::chrono::milliseconds delay(5);
		RobotStateEstimator &est = RobotStateEstimator::get();
		est.setDriveBase(this);

		while (m_running)
		{
			double time = frc::Timer::GetFPGATimestamp();
			est.oneLoop(time);
			std::this_thread::sleep_for(delay);
		}
	}

	void DriveBase::controlThread()
	{
		xerolib::XeroRobotBase &robot = getRobot();
		std::chrono::milliseconds delay(5);
		double now;

#ifdef LOGPATH
		std::ofstream pathlog("/home/pi/logs/pathlog.csv");
		pathlog << "t,pose_x,pose_y,pose_theta,linear_disp,linear_vel,profile_disp,profile_vel,vel_cmd_dx,vel_cmd_dy,vel_cmd_dtheta,steering_cmd_dx,steering_cmd_dy,steering_cmd_dtheta,cross_track_error,along_track_error,la_pt_x,la_pt_y,la_pt_vel";
		pathlog << std::endl;
#endif

		while (m_running)
		{
			now = robot.getTime();

			if (m_mode == Mode::Path)
			{
				updatePath(now);

#ifdef LOGPATH
				const xero::pathfinder::PathFollower::DebugOutput &debug = m_follower_p->getDebugOuptut();
				pathlog << debug.t;
				pathlog << "," << debug.pose_x;
				pathlog << "," << debug.pose_y;
				pathlog << "," << debug.pose_theta;
				pathlog << "," << debug.linear_displacement;
				pathlog << "," << debug.linear_velocity;
				pathlog << "," << debug.profile_displacement;
				pathlog << "," << debug.profile_velocity;
				pathlog << "," << debug.velocity_command_dx;
				pathlog << "," << debug.velocity_command_dy;
				pathlog << "," << debug.velocity_command_dtheta;
				pathlog << "," << debug.steering_command_dx;
				pathlog << "," << debug.steering_command_dy;
				pathlog << "," << debug.steering_command_dtheta;
				pathlog << "," << debug.cross_track_error;
				pathlog << "," << debug.along_track_error;
				pathlog << "," << debug.lookahead_point_x;
				pathlog << "," << debug.lookahead_point_y;
				pathlog << "," << debug.lookahead_point_velocity;
				pathlog << std::endl;
#endif
			}

			std::this_thread::sleep_for(delay);
		}
	}

	void DriveBase::velocityThread()
	{
		std::chrono::milliseconds delay(5);
		bool first = true;
		double current = frc::Timer::GetFPGATimestamp();
		double last = current;
		bool updated;

		while (m_running)
		{
			updated = false;
			current = frc::Timer::GetFPGATimestamp();

			if (m_mode == Mode::Path)
			{
				if (!first)
				{
					double lefttarget, righttarget;
					double leftactual, rightactual;
					{
						//
						// Synchronize access to the left and right target velocties
						//
						std::lock_guard<std::mutex> lock(m_velocity_mutex);
						m_left_speed.addSample(current, getLeftDistance());
						m_right_speed.addSample(current, getRightDistance());
						lefttarget = m_left_target_velocity;
						righttarget = m_right_target_velocity;
						leftactual = m_left_speed.getSpeed();
						rightactual = m_right_speed.getSpeed();
						updated = true;
					}

					double dt = current - last;
					m_left_voltage = m_left_velocity_pid.getOutput(lefttarget, leftactual, dt);
					m_right_voltage = m_right_velocity_pid.getOutput(righttarget, rightactual, dt);

					outputToMotors();
				}
			}

			if (!updated)
			{
				std::lock_guard<std::mutex> lock(m_velocity_mutex);
				m_left_speed.addSample(current, getLeftDistance());
				m_right_speed.addSample(current, getRightDistance());
			}

			first = false;
			last = current;
			std::this_thread::sleep_for(delay);
		}
	}

	void DriveBase::initPIDConstants()
	{
		ParamsParser &params = ParamsParser::get();

		double kp = params.getValue("drivebase:velocity:p");
		double ki = params.getValue("drivebase:velocity:i");
		double kd = params.getValue("drivebase:velocity:d");
		double kf = params.getValue("drivebase:velocity:f");
		double vmin = params.getValue("drivebase:velocity:vmin") ;
		double vmax = params.getValue("drivebase:velocity:vmax") ;

		m_left_velocity_pid.Init(kp, ki, kd, kf, vmin, vmax, 1e6) ;
		m_right_velocity_pid.Init(kp, ki, kd, kf, vmin, vmax, 1e6);
	}

	void DriveBase::setVelocities(double left, double right)
	{
		std::lock_guard<std::mutex> lock(m_velocity_mutex);
		m_left_target_velocity = left;
		m_right_target_velocity = right;
	}

	void DriveBase::followPath(std::shared_ptr<xero::pathfinder::Path> path_p, bool reversed)
	{
		ParamsParser &params = ParamsParser::get();

		//
		// TODO - fix so that is this is called while we are actively following a path we do
		//        the right thing.  For now, we ignore the new path and finish the current
		//        one.
		//
		if (m_mode == Mode::Path && !isPathFinished())
			return;

		double la_min = params.getValue("pathfollower:lookahead:distance:min");
		double la_max = params.getValue("pathfollower:lookahead:distance:max");
		double la_minspeed = params.getValue("pathfollower:lookahead:speed:min");
		double la_maxspeed = params.getValue("pathfollower:lookahead:speed:max");
		Lookahead la(la_min, la_max, la_minspeed, la_maxspeed);

		double igain = params.getValue("pathfollower:inertia_gain");
		double kp = params.getValue("pathfollower:kp");
		double ki = params.getValue("pathfollower:ki");
		double kv = params.getValue("pathfollower:kv");
		double kffv = params.getValue("pathfollower:kffv");
		double kffa = params.getValue("pathfollower:kffa");
		double max_abs_vel = params.getValue("pathfollower:maxvelocity");
		double max_abs_acc = params.getValue("pathfollower:maxacceleration");
		double pos_tol = params.getValue("pathfollower:position:tolerance");
		double vel_tol = params.getValue("pathfollower:velocity:tolerance");
		double stopdist = params.getValue("pathfollower:stopping:distance");
		
		PathFollower::Parameters p(la, igain, kp, ki, kv, kffv, kffa, max_abs_vel, max_abs_acc, pos_tol, vel_tol, stopdist);

		m_path_p = path_p;
		m_follower_p = std::make_shared<PathFollower>(path_p, reversed, p);

		setVelocities(0.0, 0.0);
		m_mode = Mode::Path;
	}

	double DriveBase::getDistance()
	{
		int32_t left = m_left_encoder_p->Get();
		int32_t right = m_right_encoder_p->Get();
		int32_t avg = (left + right) / 2;
		double rev = (double)avg / (double)m_ticks_per_rev;
		return rev * m_wheel_diameter * PI;
	}

	double DriveBase::getLeftDistance()
	{
		int32_t left = m_left_encoder_p->Get();
		double rev = (double)left / (double)m_ticks_per_rev;
		return rev * m_wheel_diameter * PI;
	}

	double DriveBase::getRightDistance()
	{
		int32_t right = m_right_encoder_p->Get();
		double rev = (double)right / (double)m_ticks_per_rev;
		return rev * m_wheel_diameter * PI;
	}

	void DriveBase::getInputs()
	{
		m_yaw = -m_navx_p->GetYaw();
	}

	void DriveBase::setOutputs()
	{
		if (m_mode == Mode::Manual)
			outputToMotors();
	}

	void DriveBase::updatePath(double t)
	{
		xero::pathfinder::RobotState &state = xero::pathfinder::RobotState::get();
		PositionCS pos = state.getPositionAtTime(t);
		double disp = state.getDrivenDistance();
		double vel = state.getPredictedVelocity();
		PositionAngle pa = m_follower_p->update(t, pos, disp, vel);

		if (!m_follower_p->isFinished())
		{
			double left, right;

			Kinematics::inverseKinematics(pa, m_width, m_scrub, left, right);
			setVelocities(left, right);
		}
		else
		{
			setVelocities(0.0, 0.0);
		}
	}

	void DriveBase::outputToMotors()
	{
		std::lock_guard<std::mutex> lock(m_motor_voltage_lock);

		if (std::isnan(m_left_last_voltage) || std::fabs(m_left_last_voltage - m_left_voltage) < kDelta)
		{
			for (auto motor_p : m_left_motors)
				motor_p->Set(m_left_voltage);
		}

		if (std::isnan(m_right_last_voltage) || std::fabs(m_right_last_voltage - m_right_voltage) < kDelta)
		{
			for (auto motor_p : m_right_motors)
				motor_p->Set(m_right_voltage);
		}
	}
}
