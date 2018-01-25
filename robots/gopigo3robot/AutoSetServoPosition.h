#pragma once

#include <AutonomousStepBase.h>
#include <memory>
class ServoSubsystem;

class AutoSetServoPosition : public xerolib::AutonomousStepBase
{
public:
	AutoSetServoPosition(xerolib::AutonomousControllerBase &controller, std::shared_ptr<ServoSubsystem> eyes_p, float angle);
	~AutoSetServoPosition();

	virtual void start();
	virtual bool run();
	virtual void end();
	virtual std::string toString();

private:
	std::shared_ptr<ServoSubsystem> m_eyes_p;
	float m_angle;
};

