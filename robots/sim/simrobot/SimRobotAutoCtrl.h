#pragma once

#include <AutonomousControllerBase.h>

class SimRobotAutoCtrl : public xerolib::AutonomousControllerBase
{
public:
	SimRobotAutoCtrl(xerolib::XeroRobotBase &robot);
	~SimRobotAutoCtrl();
};
