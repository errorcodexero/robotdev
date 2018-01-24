#include "Robot2018Autonomous.h"
#include <AutoSetMotorVoltage.h>
#include <AutoDriveDistance.h>
#include <AutoDelay.h>
#include <AutoDriveBaseChar.h>
#include <XeroRobotBase.h>
#include <iostream>

using namespace xerolib;


Robot2018Autonomous::Robot2018Autonomous(xerolib::XeroRobotBase &robot) 
	: xerolib::AutonomousControllerBase(robot)
{
}

Robot2018Autonomous::~Robot2018Autonomous()
{
}
