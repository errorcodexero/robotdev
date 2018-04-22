#pragma once

#include <XeroRobotBase.h>
#include <DriveBase.h>
#include "SimRobotAutoCtrl.h"

class SimRobot : public xerolib::XeroRobotBase
{
public:
	SimRobot();
	virtual ~SimRobot();

	virtual void RobotInit();

protected:

	virtual std::shared_ptr<xerolib::RobotControllerBase> createAutonomousController()
	{
		auto ctrl_p = std::make_shared<SimRobotAutoCtrl>(*this);
		return ctrl_p;
	}

	virtual std::shared_ptr<xerolib::RobotControllerBase> createOperatorControlController()
	{
		return nullptr;
	}

	virtual std::shared_ptr<xerolib::RobotControllerBase> createTestController()
	{
		return nullptr;
	}

private:
	std::shared_ptr<xerolib::DriveBase> m_drivebase_p;
};
