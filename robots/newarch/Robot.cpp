#include "Robot.h"

namespace xero {
	namespace phoenix {
		void Robot::robotLoop() {
			double initial_time = frc::Timer::GetFPGATimestamp();

			for (SubsystemPtr& subsystem : subsystems_) {
				subsystem->computeState();
			}
			controller_.run();
			for (SubsystemPtr& subsystem : subsystems_) {
				subsystem->run();
			}

			double elapsed_time = frc::Timer::GetFPGATimestamp() - initial_time > TARGET_LOOP_TIME_;
			if (elapsed_time < TARGET_LOOP_TIME_) {
				frc::Wait(TARGET_LOOP_TIME_ - elapsed_time);
			} else if (elapsed_time > TARGET_LOOP_TIME_) {
				std::cout << "Robot loop exceeded target loop time\n";
				std::cout << "Loop time: " << elapsed_time << "\n";
			}
		}

		void Robot::RobotInit() {}

		void Robot::Autonomous() {
			controller_ = AutoController();
			
			while (IsAutonomous() && IsEnabled())
				robotLoop();
		}

		void Robot::OperatorControl() {
			controller_ = TeleopController();

			while (IsOperatorControl() && IsEnabled())
				robotLoop();
		}

		void Robot::Test() {
			controller_ = TestController();

			while (IsTest() && IsEnabled())
				robotLoop();
		}

		void Robot::Disabled() {}

		Robot::Robot() {
			subsystems_.push_back(std::make_shared<Drivebase>(*this));
		}
	}
}
