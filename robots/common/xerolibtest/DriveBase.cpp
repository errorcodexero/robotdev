#include "DriveBase.h"
#include "XeroRobotBase.h"
#include <WPILib.h>
#include <cassert>
#include <iostream>
#include <cmath>
#include <numeric>

namespace xerolib
{
    const double DriveBase::PI = 3.14159265;

    DriveBase::DriveBase(XeroRobotBase &robot) : SubsystemBase("drivebase", robot)
    {
	m_left_voltage = 0.0;
	m_right_voltage = 0.0;

	m_drive_straight_mode = StraightDrivingStrategy::Encoders;

	DataLogger &data = robot.getDataLogger();

	//
	// Create the speedometers
	//
	m_linear_speedometer_p = std::make_shared<Speedometer>(4);
	m_left_linear_speedometer_p = std::make_shared<Speedometer>(4);
	m_right_linear_speedometer_p = std::make_shared<Speedometer>(4);
	m_rotational_speedometer_p = std::make_shared<Speedometer>(4);

	//
	// The columns for the data logger
	//
	m_mode_col = data.createColumn(getName() + "-mode");
	m_left_motor_col = data.createColumn(getName() + "-lv");
	m_right_motor_col = data.createColumn(getName() + "-rv");

	//
	// The data columns for driving straight
	//
	m_straight_target_speed_col = data.createColumn(getName() + "-st-tar-spd");
	m_straight_current_speed_col = data.createColumn(getName() + "-st-cur-spd");
	m_straight_current_left_speed_col = data.createColumn(getName() + "-st-cur-l-spd");
	m_straight_current_right_speed_col = data.createColumn(getName() + "-st-cur-r-spd");
	m_straight_target_dist_col = data.createColumn(getName() + "-st-tar-dst");
	m_straight_current_dist_col = data.createColumn(getName() + "-st-cur-dst");
	m_straight_current_left_dist_col = data.createColumn(getName() + "-st-cur-l-dst");
	m_straight_current_right_dist_col = data.createColumn(getName() + "-st-cur-r-dst");
	m_straight_current_target_dist_col = data.createColumn(getName() + "-st-cur-tar-dst");

	//
	// The data columns for rotation
	//
	m_angle_target_speed_col = data.createColumn(getName() + "-an-tar-spd");
	m_angle_current_speed_col = data.createColumn(getName() + "-an-cur-spd");
	m_angle_target_dist_col = data.createColumn(getName() + "-an-tar-dst");
	m_angle_current_dist_col = data.createColumn(getName() + "-an-cur-dst");
	m_angle_current_target_dist_col = data.createColumn(getName() + "-an-cur-tar-dst");

	//
	// Error columns
	//
	m_pid_error_col = data.createColumn(getName() + "-err");
	m_pid_derror_col = data.createColumn(getName() + "-derr");
	m_pid_sumi_col = data.createColumn(getName() + "-sumi");
	m_dist_error_col = data.createColumn(getName() + "-dist-err");
	m_drift_error_col = data.createColumn(getName() + "-drift-err");
    }

    DriveBase::~DriveBase()
    {
    }

    double DriveBase::getDistance()
    {
	int32_t left = m_left_encoder_p->Get();
	int32_t right = m_right_encoder_p->Get();
	int32_t avg = (left + right) / 2;
	double rev = (double)avg / (double)m_ticks_per_rev;
	return rev * m_wheel_diameter * PI ;
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

    double DriveBase::getAngle()
    {
	return m_angle_measure_p->getAngle();
    }

    void DriveBase::putModeInDashboard()
    {
	const char *mode_p ;
	
	switch(m_mode)
	{
	case Mode::Idle:
	    mode_p = "Idle" ;
	    break ;
	case Mode::Manual:
	    mode_p = "Manual" ;
	    break ;
	case Mode::Straight:
	    mode_p = "Straight" ;
	    break ;
	case Mode::Distance:
	    mode_p = "Distance" ;
	    break ;
	case Mode::Rotate:
	    mode_p = "Rotate" ;
	    break ;
	case Mode::Angle:
	    mode_p = "Angle" ;
	    break ;
	default:
	    mode_p = "Unkonwn" ;
	    break ;
	}

	frc::SmartDashboard::PutString("dbmode", mode_p) ;
    }

    void DriveBase::readInputs()
    {
	xerolib::XeroRobotBase &robot = getRobot();
	DataLogger &data = robot.getDataLogger();
	MessageLogger &logger = robot.getMessageLogger();
	double now = robot.getTime();

	putModeInDashboard() ;


	m_linear_speedometer_p->addSample(now, getDistance());
	data.logData(m_straight_current_dist_col, m_linear_speedometer_p->getLastSample());
	if (m_linear_speedometer_p->isValid())
	    data.logData(m_straight_current_speed_col, m_linear_speedometer_p->getSpeed());

	m_left_linear_speedometer_p->addSample(now, getLeftDistance());
	data.logData(m_straight_current_left_dist_col, m_left_linear_speedometer_p->getLastSample());
	if (m_left_linear_speedometer_p->isValid())
	    data.logData(m_straight_current_left_speed_col, m_left_linear_speedometer_p->getSpeed());

	m_right_linear_speedometer_p->addSample(now, getRightDistance());
	data.logData(m_straight_current_right_dist_col, m_right_linear_speedometer_p->getLastSample());
	if (m_right_linear_speedometer_p->isValid())
	    data.logData(m_straight_current_right_speed_col, m_right_linear_speedometer_p->getSpeed());

	m_rotational_speedometer_p->addSample(now, getAngle());
	data.logData(m_angle_current_dist_col, m_rotational_speedometer_p->getLastSample());
	if (m_rotational_speedometer_p->isValid())
	    data.logData(m_angle_current_speed_col, m_rotational_speedometer_p->getSpeed());

	if (m_mode == Mode::Distance || m_mode == Mode::Straight)
	{
	    double remaining = m_target - getDistance();
	    if (std::fabs(remaining) < m_straight_threshold)
	    {
		logger << MessageLogger::MessageType::Debug;
		logger << now;
		logger << ": drive base straight finished";
		logger << ", target " << m_target;
		logger << ", actual " << getDistance() ;
		logger << ", speed " << getLinearSpeed() ;
		logger << MessageLogger::Token::EndOfMessage;
		m_mode = Mode::Idle;
	    }
	    else if ((remaining < m_distance_switch_threshold || m_velocity_profile.isDone(now)) && m_mode == Mode::Straight)
	    {
		m_mode = Mode::Distance;
		m_main_pid.setConstants(m_distance_pid.p, m_distance_pid.i, m_distance_pid.d, m_distance_pid.f);
		m_main_pid.setIGuard(m_distance_pid.imax);

		logger << MessageLogger::MessageType::Debug;
		logger << now;
		logger << ": switched to distance PID controler";
		logger << ", remaining " << remaining;
		logger << ", velocity_profile_done  " << (m_velocity_profile.isDone(now) ? "true" : "false");
		logger << MessageLogger::Token::EndOfMessage;
	    }
	}
	else if (m_mode == Mode::Angle || m_mode == Mode::Rotate)
	{
	    double remaining = m_target - getAngle();
	    if (remaining < m_rotational_threshold)
	    {
		MessageLogger &logger = robot.getMessageLogger();
		logger << MessageLogger::MessageType::Debug;
		logger << now;
		logger << ": drive base rotate finished";
		logger << ", target " << m_target;
		logger << ", actual " << getDistance();
		logger << ", speed " << getLinearSpeed();
		logger << MessageLogger::Token::EndOfMessage;
		m_mode = Mode::Idle;
	    }
	    else if ((remaining < m_angle_switch_threshold || m_velocity_profile.isDone(now)) && m_mode == Mode::Rotate)
	    {
		m_mode = Mode::Distance;
		m_main_pid.setConstants(m_angle_pid.p, m_angle_pid.i, m_angle_pid.d, m_angle_pid.f);
		m_main_pid.setIGuard(m_angle_pid.imax);
	    }
	}
    }

    void DriveBase::setOutputs()
    {
	xerolib::XeroRobotBase &robot = getRobot();
	DataLogger &data = robot.getDataLogger();
	MessageLogger &logger = robot.getMessageLogger();
	double now = robot.getTime();
	double deltat = now - m_lasttime;

	data.logData(m_mode_col, static_cast<double>(m_mode));

	if (m_mode == Mode::Idle)
	{
	    m_left_voltage = 0.0;
	    m_right_voltage = 0.0;
	}
	else if (m_mode == Mode::Manual)
	{
	    //
	    // Leave the voltages where they are at
	    //
	}
	else if (m_mode == Mode::Straight || m_mode == Mode::Rotate)
	{
	    auto speed_p = getSpeedometer();
	    double remaining = m_target - speed_p->getLastSample();

	    if (speed_p->isValid())
	    {
		double target_distance = m_velocity_profile.getTargetDistance(now);

		double actual;
		if (m_mode == Mode::Straight)
		{
		    actual = getDistance();
		    data.logData(m_straight_current_target_dist_col, target_distance);
		}
		else
		{
		    actual = getAngle();
		    data.logData(m_angle_current_target_dist_col, target_distance);
		}

		double distance_error = target_distance - actual;
		if (std::fabs(distance_error) > m_error_threshold)
		    m_velocity_profile.reset();

		data.logData(m_dist_error_col, distance_error);

		// Possibly create/update the speed profile.  This is an aged speed profile, so if it already exists
		// and has not gotten too old, we just use the profile that exists.
		//
		double speed = speed_p->getSpeed();
		if (m_velocity_profile.calcProfile(now, speed, 0.0, remaining))
		{
		    logger << MessageLogger::MessageType::Debug;
		    logger << now;
		    logger << ": velocity profile recalculated";
		    logger << ", speed=" << speed_p->getSpeed();
		    logger << ", remaining=" << remaining;
		    logger << ", ta=" << m_velocity_profile.getTA();
		    logger << ", tc=" << m_velocity_profile.getTC();
		    logger << ", td=" << m_velocity_profile.getTD();
		    logger << ", vt=" << m_velocity_profile.getVT();
		    logger << MessageLogger::Token::EndOfMessage;

		    logger << MessageLogger::MessageType::Debug;
		    logger << now;
		    logger << "                               ";
		    logger << ", error=" << distance_error;
		    logger << ", target=" << target_distance;
		    logger << ", actual=" << actual;
		    logger << MessageLogger::Token::EndOfMessage;
		}

		//
		// Get the target speed
		//
		double target = m_velocity_profile.getTargetSpeed(now);

		if (m_mode == Mode::Straight)
		{
		    data.logData(m_straight_target_speed_col, target);
		    data.logData(m_straight_target_dist_col, m_target);
		}
		else
		{
		    data.logData(m_angle_target_speed_col, target);
		    data.logData(m_angle_target_dist_col, m_target);
		}

		//
		// Ask the PID controller for the motor voltage value for the motor in order for the
		// robot speed to track the speed profile.
		//
		double motor = m_main_pid.calcOutput(deltat, target, speed_p->getSpeed());
		data.logData(m_pid_error_col, m_main_pid.getLastError());
		data.logData(m_pid_derror_col, m_main_pid.getLastDerError());
		data.logData(m_pid_sumi_col, m_main_pid.getLastSumI());

		//
		// Ask an angular PID controller for the offset to the motors to cause the motors to drive straight.  If
		// we are in rotate mode, the PID constants for the m_angle_pid are set to zero so the offset is always
		// zero.
		//
		double offset;
		double right_direction = -1.0;

		if (m_mode == Mode::Straight)
		{
		    double drift = getDrift();
		    data.logData(m_drift_error_col, drift);
		    offset = m_ang_corr_pid.calcOutput(deltat, 0.0, drift);
		    right_direction = 1.0;
		}

		//
		// In C++-17 we get std::clamp, but for now we provide our own
		//
		m_left_voltage = DriveBase::clamp(motor + offset, -1.0, 1.0);
		m_right_voltage = DriveBase::clamp(right_direction * motor - offset, -1.0, 1.0);
	    }
	}
	else if (m_mode == Mode::Distance)
	{
	    double dist = getDistance();
	    double motor = m_main_pid.calcOutput(deltat, m_target, dist);

	    double drift = getDrift();
	    data.logData(m_drift_error_col, drift);
	    double mult = m_ang_corr_pid.calcOutput(deltat, 0.0, drift);

	    motor = (1.0 + mult);

	    m_left_voltage = DriveBase::clamp(motor * (1.0 + mult), -1.0, 1.0);
	    m_right_voltage = DriveBase::clamp(motor * (1.0 - mult), -1.0, 1.0);

	    data.logData(m_straight_target_dist_col, m_target);
	}
	else if (m_mode == Mode::Angle)
	{
	    double motor = m_main_pid.calcOutput(deltat, m_target, getAngle());
	    m_left_voltage = motor;
	    m_right_voltage = -motor;

	    data.logData(m_angle_target_dist_col, m_target);
	}

	data.logData(m_left_motor_col, m_left_voltage);
	data.logData(m_right_motor_col, m_right_voltage);

	setMotors();

	m_lasttime = now;
    }

    void DriveBase::setMotors()
    {
	for (auto motor_p : m_left_motors)
	    motor_p->Set(m_left_voltage);

	for (auto motor_p : m_right_motors)
	    motor_p->Set(m_right_voltage);
    }

    double DriveBase::getDrift()
    {
	double ret = 0.0;

	if (m_drive_straight_mode == StraightDrivingStrategy::AngleMeasurement)
	    ret = getAngle();
	else
	{
	    if (m_left_linear_speedometer_p->isValid() && m_right_linear_speedometer_p->isValid())
		ret = m_left_linear_speedometer_p->getSpeed() - m_right_linear_speedometer_p->getSpeed();
	}

	return ret;
    }

    void DriveBase::driveStraight(double distance)
    {
	m_mode = Mode::Straight;
	m_target = distance;

	if (distance < 0)
	{
	    m_velocity_profile.init(-m_straight_params.acceleration, -m_straight_params.deceleration, -m_straight_params.maxspeed);
	}
	else
	{
	    m_velocity_profile.init(m_straight_params.acceleration, m_straight_params.deceleration, m_straight_params.maxspeed);
	}

	m_velocity_profile.setMaxAge(m_straight_params.maxage);

	m_main_pid.setConstants(m_straight_pid.p, m_straight_pid.i, m_straight_pid.d, m_straight_pid.f);
	m_main_pid.setIGuard(m_straight_pid.imax);

	m_ang_corr_pid.setConstants(m_angle_correction_pid.p, m_angle_correction_pid.i, m_angle_correction_pid.d, m_angle_correction_pid.f);
	m_ang_corr_pid.setIGuard(m_angle_correction_pid.imax);

	resetState();
    }

    void DriveBase::rotate(double angle)
    {
	m_mode = Mode::Rotate;
	m_target = angle;

	if (angle < 0)
	{
	    m_velocity_profile.init(-m_rotational_params.acceleration, -m_rotational_params.deceleration, -m_rotational_params.maxspeed);
	}
	else
	{
	    m_velocity_profile.init(m_rotational_params.acceleration, m_rotational_params.deceleration, m_rotational_params.maxspeed);
	}

	m_velocity_profile.setMaxAge(m_rotational_params.maxage);

	m_main_pid.setConstants(m_rotational_pid.p, m_rotational_pid.i, m_rotational_pid.d, m_rotational_pid.f);
	m_main_pid.setIGuard(m_rotational_pid.imax);

	//
	// We turn off angular correction used while driving straight when we are rotating to an angle
	//
	m_ang_corr_pid.setConstants(0.0, 0.0, 0.0, 0.0);
	m_ang_corr_pid.setIGuard(0.0);

	resetState();

    }
}
