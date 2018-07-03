#pragma once

#include <WPILib.h>
#include <memory>

namespace xero {
	namespace base {

		//
		// Forward reference to the Subsystem class
		//
		class Subsystem ;
		class XeroController ;


		/// \brief the base robot class for Error Code Xero robots
		class Robot : public frc::SampleRobot {
		public:
			Robot() ;
			virtual void RobotInit() ;
			virtual void Autonomous() ;
			virtual void OperatorControl() ;
			virtual void Test() ;
			virtual void Disabled() ;


		protected:
			/// \brief this method runs one loop for the robot.
			virtual void robotLoop();

			//
			// These methods are overridden by the actual robot class because
			// the controllers are specific to a given robot
			//
			virtual std::shared_ptr<XeroController> createAutoController() = 0 ;
			virtual std::shared_ptr<XeroController> createTeleopController() = 0 ;
			virtual std::shared_ptr<XeroController> createTestController() = 0 ;

		private:
			//
			// Useful types, moved these into the class so we don't
			// pollute the global name space
			//
			typedef std::shared_ptr<Subsystem> SubsystemPtr;
			typedef std::list<SubsystemPtr> SubsystemList;
			
		private:
			// The time per robot loop in seconds
			double target_loop_time_;


			// The controller that provides control during the
			// robot loop
			std::shared_ptr<XeroController> controller_;

			// The list of subsystem that belong to the robot
			SubsystemList subsystems_;
		} ;
	}
}
