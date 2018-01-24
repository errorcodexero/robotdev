#pragma once

#include <AutonomousControllerBase.h>

class Robot2018Autonomous : public xerolib::AutonomousControllerBase
{
public:
	Robot2018Autonomous(xerolib::XeroRobotBase &robot);
	~Robot2018Autonomous();
};

