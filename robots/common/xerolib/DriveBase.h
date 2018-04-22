#pragma once

#include "SubsystemBase.h"
#include "SpeedController.h"
#include "Encoder.h"
#include "XeroRobotBase.h"
#include "Speedometer.h"
#include "PIDCtrl.h"
#include "Path.h"
#include "PathFollower.h"
#include "AHRS.h"
#include "Rotation.h"
#include <list>
#include <memory>
#include <cmath>
#include <thread>
#include <mutex>

namespace xerolib
{
    class XeroRobotBase;

    class DriveBase : public SubsystemBase
    {
	public:
		static constexpr double PI = 3.14159265;

    public:
		DriveBase(XeroRobotBase &robot,
				  std::shared_ptr<frc::SpeedController> left_motor_p, std::shared_ptr<frc::SpeedController> right_motor_p,
				  std::shared_ptr<frc::Encoder> left_enc_p,  std::shared_ptr<frc::Encoder> right_enc_p,
				  std::shared_ptr<AHRS> navx_p) ;
		DriveBase(XeroRobotBase &robot,
				  std::list<std::shared_ptr<frc::SpeedController>> left_motors, std::list<std::shared_ptr<frc::SpeedController>> right_motors,
				  std::shared_ptr<frc::Encoder> left_enc_p,  std::shared_ptr<frc::Encoder> right_enc_p,
				  std::shared_ptr<AHRS> navx_p) ;
		virtual ~DriveBase();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// Initializaion methods for the drive base
		//
		////////////////////////////////////////////////////////////////////////////////////////////////////

		void setPhysicalChar(int32_t ticks, double diam, double width, double scrub)
		{
			m_ticks_per_rev = ticks;
			m_wheel_diameter = diam;
			m_width = width;
			m_scrub = scrub;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// Misc methods for the drivebase
		//
		////////////////////////////////////////////////////////////////////////////////////////////////////

		/// \brief return the distance traveled since the last time encoders were reset
		/// \returns distance traveled since the last encoder reset
		double getDistance();

		double getLeftDistance();
		double getRightDistance();

		double getLeftSpeed()
		{
			std::lock_guard<std::mutex> lock(m_velocity_mutex);
			return m_left_speed.getSpeed();
		}

		double getRightSpeed()
		{
			std::lock_guard<std::mutex> lock(m_velocity_mutex);
			return m_right_speed.getSpeed();
		}

		xero::math::Rotation getAngle()
		{
			return xero::math::Rotation::fromDegrees(m_navx_p->GetYaw());
		}

		/// \brief returns true if the drivebase is idle
		/// \returns true if the drive base is idle
		bool isIdle()
		{
			return m_mode == Mode::Idle;
		}

		double getWidth() const
		{
			return m_width;
		}

		double getScrubFactor() const
		{
			return m_scrub;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// Subsystem methods for the drivebase
		//
		////////////////////////////////////////////////////////////////////////////////////////////////////

		/// \brief read hardware inputs and calculate state that can be used by controllers
		virtual void getInputs();

		/// \brief set hardware outputs based on desired goals of the subsystem
		virtual void setOutputs();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// High level directives for the drive base
		//
		////////////////////////////////////////////////////////////////////////////////////////////////////

		/// \brief set the drivebase motors to stopped
		void stop()
		{
			m_mode = Mode::Idle;
			setOutputVoltage(0.0, 0.0);
			m_path_p = nullptr;
			m_follower_p = nullptr;
			outputToMotors();
		}

		/// \brief set the left and right motor voltages to a fixed voltage
		/// \param left left motor voltage
		/// \param right right motor voltage
		void setMotorVoltage(double left, double right)
		{
			m_mode = Mode::Manual;
			setOutputVoltage(left, right);
			m_path_p = nullptr;
			m_follower_p = nullptr;
		}

		/// \brief set the drivebase to follow the provided path
		/// \param path_p the path to follow
		void followPath(std::shared_ptr<xero::pathfinder::Path> path_p, bool reversed);

		/// \brief returns true if the drivebase has finished following a path
		/// \returns true if the current path is finished, or we are not in path following mode
		bool isPathFinished()
		{
			if (m_mode != Mode::Path)
				return true;

			return m_follower_p->isFinished();
		}

    private:
		static constexpr double kDelta = 1.0e-6;

		enum class Mode
		{
			Idle,			// The drivebase is idle
			Manual,			// Under manual control, motor voltages can be set
			Path,			// Using pathfinder follow a path
			Velocity,		// Drive straight at a fixed velocity
		};

		DriveBase(XeroRobotBase &robot) ;

		void setMotors(std::shared_ptr<frc::SpeedController> left_p, std::shared_ptr<frc::SpeedController> right_p)
		{
			m_left_motors.push_back(left_p);
			m_right_motors.push_back(right_p);
			startThreads();
		}

		void setMotors(std::list<std::shared_ptr<frc::SpeedController>> left, std::list<std::shared_ptr<frc::SpeedController>> right)
		{
			m_left_motors = left;
			m_right_motors = right;
			startThreads();
		}

		void setEncoders(std::shared_ptr<frc::Encoder> left_p, std::shared_ptr<frc::Encoder> right_p)
		{
			m_left_encoder_p = left_p;
			m_right_encoder_p = right_p;

			resetEncoderValues();
			startThreads();
		}

		void setNavX(std::shared_ptr<AHRS> navx_p)
		{
			m_navx_p = navx_p;
			m_navx_p->ZeroYaw();
			startThreads();
		}

		static double clamp(double v, double vmin, double vmax, double prev, double chg)
		{
			double desired = std::max(vmin, std::min(v, vmax));
			double delta = std::fabs(desired - prev) ;
			if (delta > chg)
			{
				if (desired > chg)
					desired = prev + chg ;
				else
					desired = prev - chg ;
			}

			return desired ;
		}

		void startThreads();

		void resetEncoderValues()
		{
			m_left_encoder_p->Reset();
			m_right_encoder_p->Reset();
		}

		void resetNavX()
		{
			m_navx_p->ZeroYaw();
		}

		void resetState()
		{
			resetEncoderValues();
			resetNavX();
		}

		void updatePath(double t);
		void setVelocities(double left, double right);
		void outputToMotors();

		void velocityThread();
		void controlThread();
		void stateThread();
		void initPIDConstants();

		void setOutputVoltage(double left, double right)
		{
			std::lock_guard<std::mutex> lock(m_motor_voltage_lock);
			m_left_voltage = left;
			m_right_voltage = right;
		}

    private:
		//
		// While true, the threads controlling the drive base are running
		//
		bool m_running;

		//
		// The thread managing the velocity PID
		//
		std::thread m_velocity_thread;

		//
		// The mutex protecting the interface to the velocity PID thread
		//
		std::mutex m_velocity_mutex;

		//
		// The thread managing the robot state estimator
		//
		std::thread m_estimator_thread;

		//
		// The thread running the drive base
		//
		std::thread m_drivebase_control_thread;

		//
		// The lock for the output motor voltage
		//
		std::mutex m_motor_voltage_lock;

		//
		// Diameter of the wheel in inches
		//
		double m_wheel_diameter;

		//
		// The number of ticks per inch for the encoders
		//
		int32_t m_ticks_per_rev;

		//
		// The width of the robot in inches, middle of wheel to middle of wheel
		//
		double m_width;

		//
		// The scrub factor for the drivebase
		//
		double m_scrub;

		//
		// The path we are following
		//
		std::shared_ptr<xero::pathfinder::Path> m_path_p;

		//
		// The path follower object following the above path
		//
		std::shared_ptr<xero::pathfinder::PathFollower> m_follower_p;

		//
		// The mutex for setting the path and the follower
		//
		std::mutex m_path_mutex;

		// The mode for the drive base
		Mode m_mode;

		// The voltages to apply to the left and right side of the robot.  If the
		// robot is in manual mode, the voltages are given by the controller.  If the
		// robot is in automatic or rotate mode, then the voltages are controlled by
		// the drive base.
		double m_left_voltage;
		double m_right_voltage;

		double m_left_last_voltage;
		double m_right_last_voltage;

		//
		// The left and right target velocity
		//
		double m_left_target_velocity;
		double m_right_target_velocity;

		//
		// PID Controllers for the left and right sides of the robot
		//
		PIDCtrl m_left_velocity_pid;
		PIDCtrl m_right_velocity_pid;

		//
		// The speedometer for the left and right sides of the robot
		//
		Speedometer m_left_speed;
		Speedometer m_right_speed;


		////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// Drive Base hardware
		//
		////////////////////////////////////////////////////////////////////////////////////////////////////

		// The list of motors on the left side of the robot
		std::list<std::shared_ptr<frc::SpeedController>> m_left_motors;

		// The list of motors on the right side of the robot
		std::list<std::shared_ptr<frc::SpeedController>> m_right_motors;

		// The encoders on the left side of the robot
		std::shared_ptr<frc::Encoder> m_left_encoder_p;

		// The encoders on the right side of the robot
		std::shared_ptr<frc::Encoder> m_right_encoder_p;

		// The navx
		std::shared_ptr<AHRS> m_navx_p;
    };
}
