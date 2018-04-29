#include "SimRobotAutoCtrl.h"
#include "RoundedRectanglePathContainer.h"
#include "StraightLinePathContainer.h"
#include "LineArcLinePathContainer.h"
#include <XeroRobotBase.h>
#include <DriveBase.h>
#include <AutoPath.h>
#include <AutoSetMotorVoltage.h>
#include <AutoPrintPosition.h>
#include <AutoSetVelocity.h>
#include <cassert>

#define LINE_ARC_PATH_FINDER

#ifdef ROUNDED_RECT_PATH_FINDER
RoundedRectanglePathContainer rect(64, 48, 24);
#endif

#ifdef STRAIGHT_PATH_FINDER
StraightLinePathContainer line(100, 24);
#endif

#ifdef LINE_ARC_PATH_FINDER
LineArcLinePathContainer linearc;
#endif

SimRobotAutoCtrl::SimRobotAutoCtrl(xerolib::XeroRobotBase &robot) : xerolib::AutonomousControllerBase(robot)
{
	std::shared_ptr<xerolib::AutonomousStepBase> step_p;

	auto sub_p = robot.getSubsystem("drivebase");
	assert(sub_p != nullptr);
	auto db_p = std::static_pointer_cast<xerolib::DriveBase>(sub_p);

#ifdef FIXED_VELOCITIES
	step_p = std::make_shared<xerolib::AutoSetVelocity>(*this, db_p, 3.0, 80, 80);
	addStep(step_p);
	step_p = std::make_shared<xerolib::AutoSetVelocity>(*this, db_p, 3.0, 40, 40);
	addStep(step_p);
	step_p = std::make_shared<xerolib::AutoSetVelocity>(*this, db_p, 3.0, 20, 20);
	addStep(step_p);
#endif

#ifdef STRAIGHT_PATH_FINDER
	step_p = std::make_shared<xerolib::AutoPath>(*this, db_p, line, 36.0);
	addStep(step_p);
#endif

#ifdef ROUNDED_RECT_PATH_FINDER
	step_p = std::make_shared<xerolib::AutoPath>(*this, db_p, rect, 36.0);
	addStep(step_p);
#endif

#ifdef LINE_ARC_PATH_FINDER
	step_p = std::make_shared<xerolib::AutoPath>(*this, db_p, linearc, 120.0);
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

#ifdef RUN_MOTORS_VELOCITY
	step_p = std::make_shared<xerolib::AutoSetVelocity>(*this, db_p, 5.0, 10.0, 14.0);
	addStep(step_p);
#endif

#ifdef PRINT_POSITIONS
	step_p = std::make_shared<xerolib::AutoPrintPosition>(*this);
	addStep(step_p);
#endif
}


SimRobotAutoCtrl::~SimRobotAutoCtrl()
{
}
