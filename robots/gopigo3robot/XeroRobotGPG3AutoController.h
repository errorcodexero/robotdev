#pragma once

#include <AutonomousControllerBase.h>

class XeroRobotGPG3AutoController : public xerolib::AutonomousControllerBase
{
public:
	XeroRobotGPG3AutoController(xerolib::XeroRobotBase &robot);
	~XeroRobotGPG3AutoController();

private:
};

