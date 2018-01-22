#pragma once

#include "RobotBase.h"
#include <thread>

namespace frc
{
	class SampleRobot : public RobotBase
	{
	public:
		SampleRobot();
		virtual ~SampleRobot();

		virtual void StartCompetition();

		virtual void RobotInit();
		virtual void Disabled();
		virtual void Autonomous();
		virtual void OperatorControl();
		virtual void Test();
		virtual void RobotMain();

	private:
		void Controller();
		bool ProcessCmdLineArgs();

	private:
		bool m_robotMainOverridden;
		std::thread m_controller;

		bool m_running;

		double m_start_delay;
		double m_auto_period;
		double m_teleop_period;
	};
}

