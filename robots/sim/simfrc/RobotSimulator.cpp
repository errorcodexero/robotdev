#include "RobotSimulator.h"
#include "Timer.h"
#include <cassert>
#include <iostream>

namespace frc
{
	RobotSimulator *RobotSimulator::m_robotsim_p = nullptr;

	RobotSimulator::RobotSimulator()
	{
		m_left = 0.0;
		m_right = 0.0;
		m_angle = 0.0;
		m_ticks_per_rev = 200;
		m_diameter = 5.0;
		m_last_time = 0.0;
		m_left_volts = 0.0;
		m_right_volts = 0.0;
		m_scrub = 1.0;
		m_width = 24.0;
		m_right_rps_per_volt_per_time = 9.16;
		m_left_rps_per_volt_per_time = 9.16;

		m_run_thread = std::thread(&RobotSimulator::run, this);
	}

	RobotSimulator::~RobotSimulator()
	{
		m_waiting = true;
		m_running = false;
		while (m_waiting);
	}

	RobotSimulator &RobotSimulator::get()
	{
		if (m_robotsim_p == nullptr)
			m_robotsim_p = new RobotSimulator();

		return *m_robotsim_p;
	}


	void RobotSimulator::zeroYaw()
	{
		std::lock_guard<std::mutex> lock(m_lock);
		m_angle = 0.0;
	}

	double RobotSimulator::getYaw()
	{
		std::lock_guard<std::mutex> lock(m_lock);
		return m_angle;
	}

	void RobotSimulator::setMotor(int index, double v)
	{
		std::lock_guard<std::mutex> lock(m_lock);

		if (index == 0)
			m_left_volts = v;
		else if (index == 1)
			m_right_volts = v;
		else
			assert(false);
	}

	void RobotSimulator::resetEncoder(int first, int second)
	{
		std::lock_guard<std::mutex> lock(m_lock);
		if (first == 0 && second == 1)
		{
			m_left = 0.0;
		}
		else if (first == 2 && second == 3)
		{
			m_right = 0.0;
		}
		else
		{
			assert(false);
		}
	}

	int32_t RobotSimulator::getEncoder(int first, int second)
	{
		double dist;

		std::lock_guard<std::mutex> lock(m_lock);
		if (first == 0 && second == 1)
		{
			dist = m_left;
		}
		else if (first == 2 && second == 3)
		{
			dist = m_right;
		}
		else
		{
			assert(false);
		}

		double revs = dist / (PI * m_diameter);
		return static_cast<int32_t>(revs * m_ticks_per_rev);
	}

	void RobotSimulator::run()
	{
		std::chrono::microseconds delay(500);
		while (m_running)
		{
			calcPosition();
			std::this_thread::sleep_for(delay);
		}

		m_waiting = false;
	}

	void RobotSimulator::updateRobotPosition(double dleft, double dright)
	{
		m_left += dleft;
		m_right += dright;

		double dv = (dright - dleft) / 2 * m_scrub;
		m_angle += (dv * 2.0) / m_width;
	}

	void RobotSimulator::calcPosition()
	{
		std::lock_guard<std::mutex> lock(m_lock);

		//
		// We know the left and right motors have been running at a 
		// given voltage since the m_last_time_value 
		//
		double now = frc::Timer::GetFPGATimestamp();
		double elapsed = now - m_last_time;
		double dleft = m_left_volts * m_left_rps_per_volt_per_time * elapsed * m_diameter * PI;
		double dright = m_right_volts * m_right_rps_per_volt_per_time * elapsed * m_diameter * PI;

		updateRobotPosition(dleft, dright);
		m_last_time = now;
	}
}
