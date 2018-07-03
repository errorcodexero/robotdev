#include "Phoenix.h"

using namespace xero::base ;

namespace xero {
	namespace phoenix {
		void Phoenix::RobotInit() {
			//
			// This is where the subsystems for the robot get created
			//
		}

		std::shared_ptr<XeroController> Phoenix::createAutoController() {
			//
			// This is where the autonomous controller is created
			//
			return nullptr ;
		}
		
		std::shared_ptr<XeroController> Phoenix::createTeleopController() {
			//
			// This is where the teleop controller is created
			//
			return nullptr ;
		}
		 
		std::shared_ptr<XeroController> Phoenix::createTestController() {
			//
			// This is where the test controller is created
			//
			return nullptr ;
		}
	}
}

START_ROBOT_CLASS(xero::phoenix::Phoenix) ;


