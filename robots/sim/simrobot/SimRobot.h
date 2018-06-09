#pragma once

#include <XeroRobotBase.h>
#include <DriveBase.h>
#include <OdometrySystem.h>
#include <PathFollowerController.h>
#include "SimRobotAutoCtrl.h"

class SimRobot : public xero::base::XeroRobotBase
{
public:
	SimRobot();
	virtual ~SimRobot();

	virtual void RobotInit();

protected:

	virtual std::shared_ptr<xero::base::RobotControllerBase> createAutonomousController()
	{
		auto ctrl_p = std::make_shared<SimRobotAutoCtrl>(*this);
		return ctrl_p;
	}

	virtual std::shared_ptr<xero::base::RobotControllerBase> createOperatorControlController()
	{
		return nullptr;
	}

	virtual std::shared_ptr<xero::base::RobotControllerBase> createTestController()
	{
		return nullptr;
	}

private:
	std::shared_ptr<xero::base::DriveBase> createDriveBase();
	std::shared_ptr<xero::base::OdometrySystem> createOdometryController();
	std::shared_ptr<xero::base::PathFollowerController> createFollower();
};
