#include "XeroRobotGPG3AutoController.h"
#include "AutoSetServoPosition.h"
#include "ServoSubsystem.h"
#include "RoundedRectanglePathContainer.h"
#include <DriveBase.h>
#include <XeroRobotBase.h>
#include <AutoPath.h>
#include <AutoSetMotorVoltage.h>

using namespace xerolib;

RoundedRectanglePathContainer pathcontain;

XeroRobotGPG3AutoController::XeroRobotGPG3AutoController(xerolib::XeroRobotBase &robot) 
	: xerolib::AutonomousControllerBase(robot)
{
	auto sub_p = robot.getSubsystem("drivebase");
	assert(sub_p != nullptr);

	auto db_p = std::static_pointer_cast<xerolib::DriveBase>(sub_p);
	auto step_p = std::make_shared<xerolib::AutoSetMotorVoltage>(*this, db_p, 5.0, 1.0, 1.0);
	addStep(step_p);
}

XeroRobotGPG3AutoController::~XeroRobotGPG3AutoController()
{
}

