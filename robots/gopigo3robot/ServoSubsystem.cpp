#include "ServoSubsystem.h"
#include "XeroRobotBase.h"
#include <cmath>
#include <iostream>

ServoSubsystem::ServoSubsystem(xerolib::XeroRobotBase &robot, std::shared_ptr<frc::Servo> servo_p) : xerolib::SubsystemBase("eyes", robot)
{
	setServo(servo_p);
	m_moving = false;
}

ServoSubsystem::~ServoSubsystem()
{
}

void ServoSubsystem::getInputs()
{
	xerolib::XeroRobotBase &robot = getRobot();

	if (m_moving)
	{
		double elapsed = robot.getTime() - m_start;
		if (elapsed > m_duration)
		{
			m_angle = m_target_angle;
			m_moving = false;
		}
	}
}

void ServoSubsystem::setOutputs()
{
	float diff = std::abs(m_target_angle - m_angle);
	if (!m_moving && diff > kDelta)
	{
		xerolib::XeroRobotBase &robot = getRobot();
		m_servo_p->setPosition(m_target_angle);
		m_start = robot.getTime();
		m_duration = diff * 1.0 / 180.0 + 0.25;
		m_moving = true;
	}
}

void ServoSubsystem::setAngle(float angle)
{
	m_target_angle = angle;
}