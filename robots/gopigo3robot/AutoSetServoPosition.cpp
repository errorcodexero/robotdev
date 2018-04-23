#include "AutoSetServoPosition.h"
#include "ServoSubsystem.h"
#include <sstream>

AutoSetServoPosition::AutoSetServoPosition(xerolib::AutonomousControllerBase &controller, std::shared_ptr<ServoSubsystem> eyes_p, float angle)
	: xerolib::AutonomousStepBase(controller)
{
	m_eyes_p = eyes_p;
	m_angle = angle;
}

AutoSetServoPosition::~AutoSetServoPosition()
{
}

void AutoSetServoPosition::start()
{
	m_eyes_p->setAngle(m_angle);
}

bool AutoSetServoPosition::run()
{
	return !m_eyes_p->isMoving();
}

void AutoSetServoPosition::end()
{
}

std::string AutoSetServoPosition::toString()
{
	std::stringstream strm;
	strm << "set eyes postiion voltage, angle = " <<
	m_angle << " degrees";
	return strm.str();
}

