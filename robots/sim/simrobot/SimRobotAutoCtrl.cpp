#include "SimRobotAutoCtrl.h"
#include <XeroRobotBase.h>
#include <DriveBase.h>
#include <AutoSetMotorVoltage.h>
#include <AutoPrintPosition.h>
#include <AutoSetVelocity.h>
#include <cassert>

SimRobotAutoCtrl::SimRobotAutoCtrl(xerolib::XeroRobotBase &robot) : xerolib::AutonomousControllerBase(robot)
{
	std::shared_ptr<xerolib::AutonomousStepBase> step_p;

	auto sub_p = robot.getSubsystem("drivebase");
	assert(sub_p != nullptr);
	auto db_p = std::static_pointer_cast<xerolib::DriveBase>(sub_p);

	step_p = std::make_shared<xerolib::AutoSetVelocity>(*this, db_p, 3.0, 80, 80);
	addStep(step_p);

	step_p = std::make_shared<xerolib::AutoSetVelocity>(*this, db_p, 3.0, 40, 40);
	addStep(step_p);

	step_p = std::make_shared<xerolib::AutoSetVelocity>(*this, db_p, 3.0, 40, 20);
	addStep(step_p);
}


SimRobotAutoCtrl::~SimRobotAutoCtrl()
{
}
