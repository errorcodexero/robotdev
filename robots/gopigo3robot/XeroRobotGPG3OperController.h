#pragma once

#include <RobotControllerBase.h>

class XeroRobotGPG3OperController : xerolib::RobotControllerBase
{
public:
	XeroRobotGPG3OperController(xerolib::XeroRobotBase &robot);
	virtual ~XeroRobotGPG3OperController();

	virtual void executeOneLoop();
};

