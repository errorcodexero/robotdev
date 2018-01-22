#pragma once

#include "SubsystemBase.h"
#include <Servo.h>
#include <memory>

class ServoSubsystem : public xerolib::SubsystemBase
{
public:
	ServoSubsystem(xerolib::XeroRobotBase &robot);
	~ServoSubsystem();

	void setServo(std::shared_ptr<frc::Servo> servo_p)
	{
		m_servo_p = servo_p;
	}

	virtual void readInputs();
	virtual void setOutputs();

	void setAngle(float angle);

	bool isMoving() const
	{
		return m_moving;
	}

private:
	static const float Delta;

private:
	// The current angle
	float m_angle;

	// The desired angle
	float m_target_angle;

	// If true, we are moving the servo
	bool m_moving;

	// The time to move if moving
	double m_duration;

	// The time the move started if moving
	double m_start;

	// The servo that controls the eyes
	std::shared_ptr<frc::Servo> m_servo_p;
};
