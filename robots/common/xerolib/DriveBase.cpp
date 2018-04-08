#include "DriveBase.h"
#include "XeroRobotBase.h"
#include <WPILib.h>
#include <cassert>
#include <iostream>
#include <cmath>
#include <numeric>

namespace xerolib
{
	const double DriveBase::PI = 3.14159265;

	DriveBase::DriveBase(XeroRobotBase &robot) : SubsystemBase("drivebase", robot)
	{
		m_mode = Mode::Idle;
		m_left_voltage = 0.0;
		m_right_voltage = 0.0;
		m_velocity_thread_running = false;
	}

	DriveBase::~DriveBase()
	{
	}

	void DriveBase::startVelocityThread()
	{
		if (!m_velocity_thread_running)
		{
			m_velocity_thread_running = true;
			m_thread = std::thread(&DriveBase::velocityThread, this);
		}
	}

	void DriveBase::velocityThread()
	{
		while (m_velocity_thread_running)
		{
		}
	}

	void DriveBase::setMotorVoltages(double left, double right)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_left_voltage = left;
		m_right_voltage = right;
	}

	double DriveBase::getDistance()
	{
		int32_t left = m_left_encoder_p->Get();
		int32_t right = m_right_encoder_p->Get();
		int32_t avg = (left + right) / 2;
		double rev = (double)avg / (double)m_ticks_per_rev;
		return rev * m_wheel_diameter * PI;
	}

	double DriveBase::getLeftDistance()
	{
		int32_t left = m_left_encoder_p->Get();
		double rev = (double)left / (double)m_ticks_per_rev;
		return rev * m_wheel_diameter * PI;
	}

	double DriveBase::getRightDistance()
	{
		int32_t right = m_right_encoder_p->Get();
		double rev = (double)right / (double)m_ticks_per_rev;
		return rev * m_wheel_diameter * PI;
	}

	void DriveBase::readInputs()
	{
	}

	void DriveBase::setOutputs()
	{
		xerolib::XeroRobotBase &robot = getRobot();
		DataLogger &data = robot.getDataLogger();
		MessageLogger &logger = robot.getMessageLogger();
		double now = robot.getTime();

		if (m_mode == Mode::Idle)
		{
			m_left_voltage = 0.0;
			m_right_voltage = 0.0;
		}
		else if (m_mode == Mode::Manual)
		{
			//
			// Leave the voltages where they are at
			//
		}
		else if (m_mode == Mode::Path)
		{

		}

		setMotors();
	}

	void DriveBase::setMotors()
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		for (auto motor_p : m_left_motors)
			motor_p->Set(m_left_voltage);

		for (auto motor_p : m_right_motors)
			motor_p->Set(m_right_voltage);
	}
}