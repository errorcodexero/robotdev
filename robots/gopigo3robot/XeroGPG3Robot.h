#pragma once

#include "XeroRobotBase.h"
#include "DriveBase.h"
#include "XeroRobotGPG3AutoController.h"
#include "ServoSubsystem.h"
#include <map>
#include <string>

class XeroGPG3Robot : public xerolib::XeroRobotBase
{
public:
	XeroGPG3Robot();
	~XeroGPG3Robot();

	virtual void RobotInit();

protected:
	/// @brief this method is implemented by a derived class and creates the autonomous controller
	virtual std::shared_ptr<xerolib::RobotControllerBase> createAutonomousController()
	{
		std::shared_ptr<XeroRobotGPG3AutoController> ctrl_p = std::make_shared<XeroRobotGPG3AutoController>(*this);
		return ctrl_p;
	}

	/// @brief this method is implemented by a derived class and creates the operator controller
	virtual std::shared_ptr<xerolib::RobotControllerBase> createOperatorControlController()
	{
		return nullptr;
	}

	/// @brief this method is implemented by a derived class and creates the test controller
	virtual std::shared_ptr<xerolib::RobotControllerBase> createTestController()
	{
		return nullptr;
	}

private:
	//
	// The drive base for this robot
	//
	std::shared_ptr<xerolib::DriveBase> m_drivebase_p;

	//
	// The eyes for this robot
	//
	std::shared_ptr<ServoSubsystem> m_eyes_p;

};
