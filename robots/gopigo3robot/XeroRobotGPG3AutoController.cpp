#include "XeroRobotGPG3AutoController.h"
#include "AutoSetServoPosition.h"
#include "ServoSubsystem.h"
#include "RoundedRectanglePathContainer.h"
#include <DriveBase.h>
#include <XeroRobotBase.h>
#include <AutoPath.h>

using namespace xerolib;

RoundedRectanglePathContainer pathcontain;

XeroRobotGPG3AutoController::XeroRobotGPG3AutoController(xerolib::XeroRobotBase &robot) 
	: xerolib::AutonomousControllerBase(robot)
{
	auto sub_p = robot.getSubsystem("drivebase");
	assert(sub_p != nullptr);

	auto db_p = std::static_pointer_cast<xerolib::DriveBase>(sub_p);
	std::shared_ptr<xerolib::AutoPath> step_p = std::make_shared<xerolib::AutoPath>(*this, db_p, pathcontain);
	addStep(step_p);
}

XeroRobotGPG3AutoController::~XeroRobotGPG3AutoController()
{
}

