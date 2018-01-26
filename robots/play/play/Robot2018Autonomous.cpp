#include "Robot2018Autonomous.h"
#include <AutoSetMotorVoltage.h>
#include <AutoDriveDistance.h>
#include <AutoDelay.h>
#include <AutoDriveBaseChar.h>
#include <DriveBase.h>
#include <XeroRobotBase.h>
#include <iostream>

using namespace xerolib;


Robot2018Autonomous::Robot2018Autonomous(xerolib::XeroRobotBase &robot) 
	: xerolib::AutonomousControllerBase(robot)
{
	// createMotorCharProgram();
	createDriveStraight();
}

Robot2018Autonomous::~Robot2018Autonomous()
{
}

void Robot2018Autonomous::createDriveStraight()
{
	xerolib::XeroRobotBase &robot = getRobot();
	std::shared_ptr<SubsystemBase> sub_p;

	sub_p = robot.getSubsystem("drivebase");
	std::shared_ptr<DriveBase> drive_p = std::dynamic_pointer_cast<DriveBase>(sub_p);

	addStep(std::make_shared<AutoDriveDistance>(*this, drive_p, 72.00));
}

void Robot2018Autonomous::createMotorCharProgram()
{
	xerolib::XeroRobotBase &robot = getRobot();
	std::shared_ptr<SubsystemBase> sub_p;

	sub_p = robot.getSubsystem("drivebase");
	std::shared_ptr<DriveBase> drive_p = std::dynamic_pointer_cast<DriveBase>(sub_p);

	for (double volts = 0.1; volts <= 1.0; volts += 0.1)
	{
		addStep(std::make_shared<AutoDriveBaseChar>(*this, drive_p, volts, 5.0, true));
		addStep(std::make_shared<AutoDelay>(*this, 1.0));
	}
}
