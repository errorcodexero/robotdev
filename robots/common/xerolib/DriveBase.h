#pragma once

#include "SubsystemBase.h"
#include "SpeedController.h"
#include "Encoder.h"
#include "XeroRobotBase.h"
#include "Speedometer.h"
#include "AgedTrapezoidSpeedProfile.h"
#include "PIDCtrl.h"
#include "AngleMeasurementDevice.h"
#include "Path.h"
#include "PathFollower.h"
#include <list>
#include <memory>
#include <cmath>
#include <thread>

class AHRS;

namespace xerolib
{
    class XeroRobotBase;

    class DriveBase : public SubsystemBase
    {
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
		}

		/// \brief set the left and right motor voltages to a fixed voltage
		/// \param left left motor voltage
		/// \param right right motor voltage
		void setMotorVoltage(double left, double right)
		{
			m_mode = Mode::Manual;
			setMotorVoltages(left, right);
		}

		/// \brief follow a path
		void followPath(std::shared_ptr<xero::pathfinder::Path> path_p)
		{
			m_mode = Mode::Path;
			m_path_p = path_p;
		}

    private:
		enum class Mode
		{
			Idle,			// The drivebase is idle
			Manual,			// Under manual control, motor voltages can be set
			Path,
		};

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

		void resetEncoderValues()
		{
			m_left_encoder_p->Reset();
			m_right_encoder_p->Reset();
		}

		void resetState()
		{
			resetEncoderValues();
		}

		void setMotorVoltages(double left, double right);
		void setMotors();
		void startVelocityThread();
		void velocityThread();

    private:
		//
		// PI
		//
		static const double PI;

		//
		// The thread managing the velocity PID
		//
		std::thread m_thread;

		//
		// The mutex protecting the interface to the velocity PID thread
		//
		std::mutex m_mutex;

		//
		// Physical characteristics for the drive base
		//
		double m_wheel_diameter;
		int32_t m_ticks_per_rev;
		double m_width;

		//
		// The path we are following
		//
		std::shared_ptr<xero::pathfinder::Path> m_path_p;
		std::shared_ptr<xero::pathfinder::PathFollower> m_follower_p;

		// The mode for the drive base
		Mode m_mode;

		// The voltages to apply to the left and right side of the robot.  If the
		// robot is in manual mode, the voltages are given by the controller.  If the
		// robot is in automatic or rotate mode, then the voltages are controlled by
		// the drive base.
		double m_left_voltage;
		double m_right_voltage;

		double m_left_velocity;
		double m_right_velocity;

		//
		// If true, the velocity PID thread is already running
		//
		bool m_velocity_thread_running;

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
    };
}
