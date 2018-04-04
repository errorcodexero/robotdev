#pragma once
#include "WPI_TalonSRX.h"
#include "Encoder.h"
#include "SubsystemBase.h"
#include "XeroRobotBase.h"
#include "Speedometer.h"
#include "AgedTrapezoidSpeedProfile.h"
#include "PIDCtrl.h"
#include "AngleMeasurementDevice.h"
#include <list>
#include <memory>
#include <cmath>

class AHRS;

namespace xerolib
{
    class XeroRobotBase;

    class DriveBase : public SubsystemBase
    {
    public:

    public:
		DriveBase(XeroRobotBase &robot);
		~DriveBase();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// Initializaion methods for the drive base
		//
		////////////////////////////////////////////////////////////////////////////////////////////////////

		void setMotors(std::shared_ptr<ctre::phoenix::motorcontrol::can::WPI_TalonSRX> left_p, std::shared_ptr<ctre::phoenix::motorcontrol::can::WPI_TalonSRX> right_p)
		{
			m_left_motors.push_back(left_p);
			m_right_motors.push_back(right_p);
		}

		void setMotors(std::list<std::shared_ptr<ctre::phoenix::motorcontrol::can::WPI_TalonSRX>> left, std::list<std::shared_ptr<ctre::phoenix::motorcontrol::can::WPI_TalonSRX>> right)
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

		void setPhysicalChar(int32_t ticks, double diam)
		{
			m_ticks_per_rev = ticks;
			m_wheel_diameter = diam;
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

    private:
		enum class Mode
		{
			Idle,			// The drivebase is idle
			Manual,			// Under manual control, motor voltages can be set
			FollowPath,		// Using the path follower, follow a given path
		};

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

		// The mode for the drive base
		Mode m_mode;

		// The voltages to apply to the left and right side of the robot.  If the
		// robot is in manual mode, the voltages are given by the controller.  If the
		// robot is in automatic or rotate mode, then the voltages are controlled by
		// the drive base.
		double m_left_voltage;
		double m_right_voltage;

		double m_max_voltage_change ;
		double m_max_voltage ;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// Drive Base hardware
		//
		////////////////////////////////////////////////////////////////////////////////////////////////////

		// The list of motors on the left side of the robot
		std::list<std::shared_ptr<ctre::phoenix::motorcontrol::can::WPI_TalonSRX>> m_left_motors;

		// The list of motors on the right side of the robot
		std::list<std::shared_ptr<ctre::phoenix::motorcontrol::can::WPI_TalonSRX>> m_right_motors;

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
    };
}
