#pragma once

#include <Robot.h>

namespace xero {
	namespace phoenix {

		class Phoenix : public xero::base::Robot {
			virtual void RobotInit() ;
			virtual std::shared_ptr<xero::base::XeroController> createAutoController() ;
			virtual std::shared_ptr<xero::base::XeroController> createTeleopController() ;
			virtual std::shared_ptr<xero::base::XeroController> createTestController() ;
		} ;
	}
}
