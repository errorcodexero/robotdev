#include "ServoSubsystem.h"
#include "XeroRobotBase.h"
#include <cmath>
#include <iostream>

const float ServoSubsystem::Delta = 1.0;

ServoSubsystem::ServoSubsystem(xerolib::XeroRobotBase &robot) : xerolib::SubsystemBase("eyes", robot)
{
	m_moving = false;
}

ServoSubsystem::~ServoSubsystem()
{
}

void ServoSubsystem::readInputs()
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
	if (!m_moving && diff > Delta)
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