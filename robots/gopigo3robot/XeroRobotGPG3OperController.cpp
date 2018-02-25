#include "XeroRobotGPG3OperController.h"
#include <DriverStation.h>

XeroRobotGPG3OperController::XeroRobotGPG3OperController(xerolib::XeroRobotBase &robot) : xerolib::RobotControllerBase(robot)
{
}

XeroRobotGPG3OperController::~XeroRobotGPG3OperController()
{
}

void XeroRobotGPG3OperController::executeOneLoop()
{
	frc::DriverStation &ds = frc::DriverStation::GetInstance();

	for (int i = 0; i < ds.GetStickAxisCount(0); i++)
	{
		double axis = ds.GetStickAxis(0, i);
		std::cout << "Axis Value " << axis << std::endl;
	}
}
