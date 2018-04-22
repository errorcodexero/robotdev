#pragma once

#include <cstdint>
#include <thread>
#include <mutex>

namespace frc
{
	class RobotSimulator
	{
	private:
		RobotSimulator();

	public:
		~RobotSimulator();

		static RobotSimulator &get();

		double getYaw();
		void zeroYaw();
		void resetEncoder(int first, int second);
		int32_t getEncoder(int first, int second);
		void setMotor(int index, double v);

	private:
		void run();
		void calcPosition();
		void updateRobotPosition(double dleft, double dright);

	private:
		static constexpr double PI = 3.141592;
		static RobotSimulator *m_robotsim_p;

		double m_last_time;
		double m_left_volts;
		double m_right_volts;
		double m_left_rps_per_volt_per_time;
		double m_right_rps_per_volt_per_time;

		int32_t m_ticks_per_rev;
		double m_diameter;
		double m_left;
		double m_right;
		double m_angle;
		double m_scrub;
		double m_width;

		bool m_waiting;
		bool m_running;
		std::mutex m_lock;
		std::thread m_run_thread;
	};
}

