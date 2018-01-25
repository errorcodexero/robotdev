#pragma once

#include "SubsystemBase.h"
#include "SpeedController.h"
#include "Encoder.h"
#include "XeroRobotBase.h"
#include "Speedometer.h"
#include "AgedTrapezoidSpeedProfile.h"
#include "PIDCtrl.h"
#include "AngleMeasurementDevice.h"
#include <list>
#include <memory>

class AHRS;

namespace xerolib
{
    class XeroRobotBase;

    class DriveBase : public SubsystemBase
    {
    public:
	struct MotionProfileParams
	{
	    double acceleration;
	    double deceleration;
	    double maxspeed;
	    double maxage;
	};

	struct PIDConstants
	{
	    double p;
	    double i;
	    double d;
	    double f;
	    double imax;
	};

	enum class StraightDrivingStrategy
	{
	    None,
		Encoders,
		AngleMeasurement,
		};

    public:
	DriveBase(XeroRobotBase &robot);
	~DriveBase();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Initializaion methods for the drive base
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void setMotors(std::shared_ptr<frc::SpeedController> left_p, std::shared_ptr<frc::SpeedController> right_p)
	{
	    m_left_motors.push_back(left_p);
	    m_right_motors.push_back(right_p);
	}

	void setMotors(std::list<std::shared_ptr<frc::SpeedController>> left, std::list<std::shared_ptr<frc::SpeedController>> right)
	{
	    m_left_motors = left;
	    m_right_motors = right;
	}

	void setEncoders(std::shared_ptr<frc::Encoder> left_p, std::shared_ptr<frc::Encoder> right_p)
	{
	    m_left_encoder_p = left_p;
	    m_right_encoder_p = right_p;

	    resetEncoderValues();
	}

	void setAngleMeasurementDevice(std::shared_ptr<AngleMeasurementDevice> angle_p)
	{
	    m_angle_measure_p = angle_p;
	    m_angle_measure_p->reset();
	}

	void setStraightMotionProfiles(const MotionProfileParams &straight, double threshold, double switch_dist)
	{
	    m_straight_params = straight;
	    m_straight_threshold = threshold;
	    m_distance_switch_threshold = switch_dist;
	}

	void setRotationalMotionProfiles(const MotionProfileParams &rotational, double threshold, double switch_dist)
	{
	    m_rotational_params = rotational;
	    m_rotational_threshold = threshold;
	    m_angle_switch_threshold = switch_dist;
	}

	void setStraightPIDConstants(const PIDConstants &straight, const PIDConstants &distance, const PIDConstants &angle)
	{
	    m_straight_pid = straight;
	    m_distance_pid = distance;
	    m_angle_correction_pid = angle;
	}

	void setRotationalPIDConstants(const PIDConstants &rotational, const PIDConstants &angle)
	{
	    m_rotational_pid = rotational;
	    m_angle_pid = angle;
	}

	void setPhysicalChar(int32_t ticks, double diam)
	{
	    m_ticks_per_rev = ticks;
	    m_wheel_diameter = diam;
	}

	void setErrorThreshold(double thresh)
	{
	    m_error_threshold = thresh;
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

	/// \brief return the angle traveled since the last time the angle measurement device was reset
	/// \returns the angle traveled since the last angle measurement device reset
	double getAngle();

	/// \brief returns true if the drivebase is idle
	/// \returns true if the drive base is idle
	bool isIdle()
	{
	    return m_mode == Mode::Idle;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Subsystem methods for the drivebase
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////

	/// \brief read hardware inputs and calculate state that can be used by controllers
	virtual void readInputs();

	/// \brief set hardware outputs based on desired goals of the subsystem
	virtual void setOutputs();

	/// \brief return the linear speed of the robot in inches per second
	/// \returns the linear speed of the robot
	double getLinearSpeed()
	{
	    if (!m_linear_speedometer_p->isValid())
		return 0.0;

	    return m_linear_speedometer_p->getSpeed();
	}

	/// \brief return the rotational speed of the robot in degrees per second
	/// \returns the rotational speed of the robot
	double getRotationalSpeed()
	{
	    if (!m_rotational_speedometer_p->isValid())
		return 0.0;

	    return m_rotational_speedometer_p->getSpeed();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// High level directives for the drive base
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void idle()
	{
	    m_left_voltage = 0.0;
	    m_right_voltage = 0.0;
	    m_mode = Mode::Idle;
	    resetState();
	}

	/// \brief set the left and right motor voltages to a fixed voltage
	/// \param left left motor voltage
	/// \param right right motor voltage
	void setMotorVoltage(double left, double right)
	{
	    m_mode = Mode::Manual;
	    m_left_voltage = left;
	    m_right_voltage = right;
	    resetState();
	}

	/// \brief tells the drive base to drive a specific distance.
	/// The drive base will perform this operation over multiple cycles.
	/// \param distance the distance to drive
	void driveStraight(double distance);

	/// \brief tells the drive base to rotate a specific distance
	/// The drive base will perform this operation over multiple cycles.
	/// \param angle to rotate
	void rotate(double angle);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Characterization methods to tune drivebase
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////

	/// \brief start a measurement cycle for the robot
	void startLinearMeasure()
	{
	    m_left_encoder_p->Reset();
	    m_right_encoder_p->Reset();
	}

	double endLinearMeasure()
	{
	    return getLinearSpeed();
	}

	void startRotationalMeasure()
	{
	    m_left_encoder_p->Reset();
	    m_right_encoder_p->Reset();
	}

	double endRotationalMeasure()
	{
	    return getRotationalSpeed();
	}

    private:
	enum class Mode
	{
	    Idle,			// The drivebase is idle
	    Manual,			// Under manual control, motor voltages can be set
	    Straight,			// Drive straight a linear distance under velocity control
	    Distance,			// Drive straight a linear distance under distance control
	    Rotate,			// Rotate a given angle under rotational velocity control
	    Angle,			// Rotate a given angle under rotational angle control
	};

	static double clamp(double v, double vmin, double vmax)
	{
	    return std::max(vmin, std::min(v, vmax));
	}

	double getDrift();

	void resetEncoderValues()
	{
	    m_left_encoder_p->Reset();
	    m_right_encoder_p->Reset();
	}

	void resetAngleMeasurementDevice()
	{
	    m_angle_measure_p->reset();
	}

	void resetState()
	{
	    resetEncoderValues();
	    resetAngleMeasurementDevice();
	}

	void setMotors();

	std::shared_ptr<Speedometer> getSpeedometer()
	{
	    assert(m_mode == Mode::Straight || m_mode == Mode::Rotate);

	    if (m_mode == Mode::Straight)
		return m_linear_speedometer_p;

	    return m_rotational_speedometer_p;
	}

    private:
	//
	// PI
	//
	static const double PI;

	//
	// Physical characteristics for the drive base
	//
	double m_wheel_diameter;
	int32_t m_ticks_per_rev;

	//
	// Motion profiles for various types of motion
	//
	MotionProfileParams m_straight_params;
	MotionProfileParams m_rotational_params;

	//
	// PID constants for the various PID controllers needed
	//
	PIDConstants m_straight_pid;
	PIDConstants m_distance_pid;
	PIDConstants m_angle_correction_pid;
	PIDConstants m_rotational_pid;
	PIDConstants m_angle_pid;

	//
	// Thresholds for the drivebase algorithms
	//
	double m_straight_threshold;
	double m_distance_switch_threshold;
	double m_rotational_threshold;
	double m_angle_switch_threshold;

	// The mode for the drive base
	Mode m_mode;

	// The mode for driving straight (navx vs encoders)
	StraightDrivingStrategy m_drive_straight_mode;

	// For Distance mode, the distance to cover in inches
	// For Rotate mode, the amount to rotate in degrees
	double m_target;

	// The time the last time through the robot loop
	double m_lasttime;

	// The error threshold for distance error to force a new speed profile
	double m_error_threshold;

	// The voltages to apply to the left and right side of the robot.  If the
	// robot is in manual mode, the voltages are given by the controller.  If the
	// robot is in automatic or rotate mode, then the voltages are controlled by
	// the drive base.
	double m_left_voltage;
	double m_right_voltage;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Motion profile and PID controllers
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////

	AgedTrapezoidSpeedProfile m_velocity_profile;
	PIDCtrl m_main_pid;
	PIDCtrl m_ang_corr_pid;

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

	// The angle measurement device object
	std::shared_ptr<AngleMeasurementDevice> m_angle_measure_p;

	// A speedometer for the robot, this one is linear speed
	std::shared_ptr<Speedometer> m_linear_speedometer_p;
	std::shared_ptr<Speedometer> m_left_linear_speedometer_p;
	std::shared_ptr<Speedometer> m_right_linear_speedometer_p;

	// A speedometer for the robot, this one is rotational speed
	std::shared_ptr<Speedometer> m_rotational_speedometer_p;

	//
	// The columns for the data logger
	//
	size_t m_mode_col;

	// The index for the voltages for the left and right motors
	size_t m_left_motor_col;
	size_t m_right_motor_col;

	//
	// The data columns for driving straight
	//
	size_t m_straight_target_speed_col;
	size_t m_straight_current_speed_col;
	size_t m_straight_current_left_speed_col;
	size_t m_straight_current_right_speed_col;
	size_t m_straight_target_dist_col;
	size_t m_straight_current_dist_col;
	size_t m_straight_current_left_dist_col;
	size_t m_straight_current_right_dist_col;
	size_t m_straight_current_target_dist_col;

	//
	// The data columns for rotation
	//
	size_t m_angle_target_speed_col;
	size_t m_angle_current_speed_col;
	size_t m_angle_target_dist_col;
	size_t m_angle_current_dist_col;
	size_t m_angle_current_target_dist_col;

	size_t m_pid_error_col;
	size_t m_pid_derror_col;
	size_t m_pid_sumi_col;
	size_t m_dist_error_col;
	size_t m_drift_error_col;
    };
}
