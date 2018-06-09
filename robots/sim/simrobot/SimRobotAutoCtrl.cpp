#include "SimRobotAutoCtrl.h"
#include <XeroRobotBase.h>
#include <TankDrive.h>
#include <AutoSetMotorVoltage.h>
#include <AutoSetVelocity.h>
#include <cassert>

SimRobotAutoCtrl::SimRobotAutoCtrl(xero::base::XeroRobotBase &robot) : xero::base::AutonomousControllerBase(robot)
{
	std::shared_ptr<xero::base::AutonomousStepBase> step_p;

	auto sub_p = robot.getSubsystem("TankDrive");
	assert(sub_p != nullptr);
	auto db_p = std::static_pointer_cast<xero::base::TankDrive>(sub_p);

	xero::base::PositionAngle pa(xero::base::Position(0.0, 90.0), 0.0);
	db_p->resetPosition(pa);

	step_p = std::make_shared<xero::base::AutoSetVelocity>(*this, db_p, 10.0, 36, 36);
	addStep(step_p);

	//step_p = std::make_shared<xero::base::AutoFollowPath>(*this, db_p, "CrossScale", "gen.csv");
	//addStep(step_p);
}


SimRobotAutoCtrl::~SimRobotAutoCtrl()
{
}
