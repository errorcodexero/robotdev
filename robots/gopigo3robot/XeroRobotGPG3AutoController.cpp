#include "XeroRobotGPG3AutoController.h"
#include "AutoSetServoPosition.h"
#include "ServoSubsystem.h"
#include "RoundedRectanglePathContainer.h"
#include <DriveBase.h>
#include <XeroRobotBase.h>
#include <AutoPath.h>

using namespace xerolib;

XeroRobotGPG3AutoController::XeroRobotGPG3AutoController(xerolib::XeroRobotBase &robot) 
	: xerolib::AutonomousControllerBase(robot)
{
}

XeroRobotGPG3AutoController::~XeroRobotGPG3AutoController()
{
}

