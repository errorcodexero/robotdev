#pragma once

#include <AutonomousControllerBase.h>

class SimRobotAutoCtrl : public xero::base::AutonomousControllerBase
{
public:
	SimRobotAutoCtrl(xero::base::XeroRobotBase &robot);
	~SimRobotAutoCtrl();
};
