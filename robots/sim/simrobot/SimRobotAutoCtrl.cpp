#include "SimRobotAutoCtrl.h"
#include "RoundedRectanglePathContainer.h"
#include "StraightLinePathContainer.h"
#include <XeroRobotBase.h>
#include <DriveBase.h>
#include <AutoPath.h>
#include <AutoSetMotorVoltage.h>
#include <AutoPrintPosition.h>
#include <cassert>

#define RUN_MOTORS_SPIN

RoundedRectanglePathContainer rect(64, 48, 24);
StraightLinePathContainer line(100, 24);

SimRobotAutoCtrl::SimRobotAutoCtrl(xerolib::XeroRobotBase &robot) : xerolib::AutonomousControllerBase(robot)
{
	std::shared_ptr<xerolib::AutonomousStepBase> step_p;

	auto sub_p = robot.getSubsystem("drivebase");
	assert(sub_p != nullptr);
	auto db_p = std::static_pointer_cast<xerolib::DriveBase>(sub_p);

#ifdef STRAIGHT_PATH_FINGER
	step_p = std::make_shared<xerolib::AutoPath>(*this, db_p, line);
	addStep(step_p);
#endif

#ifdef ROUNDED_RECT_PATH_FINDER
	step_p = std::make_shared<xerolib::AutoPath>(*this, db_p, rect);
	addStep(step_p);
#endif

#ifdef RUN_MOTORS_STRAIGHT
	step_p = std::make_shared<xerolib::AutoSetMotorVoltage>(*this, db_p, 5.0, 0.25, 0.25);
	addStep(step_p);
#endif

#ifdef RUN_MOTORS_SPIN
	step_p = std::make_shared<xerolib::AutoSetMotorVoltage>(*this, db_p, 5.0, -0.25, 0.25);
	addStep(step_p);
#endif

	step_p = std::make_shared<xerolib::AutoPrintPosition>(*this);
	addStep(step_p);
}


SimRobotAutoCtrl::~SimRobotAutoCtrl()
{
}
