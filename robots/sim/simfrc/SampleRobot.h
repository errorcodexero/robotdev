#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <thread>

#define START_ROBOT_CLASS(_ClassName_)                                               \
  int main(int ac, char **av) {                                                      \
    _ClassName_ *robot = new _ClassName_();                                          \
	while (ac-- > 0) robot->addArg(*av++) ;			         						 \
	try {									         								 \
        frc::SampleRobot::robotSetup(robot);                                         \
    }                                                                                \
    catch (const std::exception &ex)                                                 \
    {																		         \
        std::cout << "exception caught from robot - " << ex.what() << std::endl ;    \
    }                                                                                \
    return 0;                                                                        \
  }

namespace frc
{
	class SampleRobot
	{
	public:
		SampleRobot();
		~SampleRobot();

		void addArg(const char *arg_p)
		{
			m_args.push_back(arg_p);
		}

		static void robotSetup(SampleRobot *robot_p);

		bool IsEnabled() const;
		bool IsFinished() const;
		bool IsDisabled() const;
		bool IsAutonomous() const;
		bool IsTest() const;
		bool IsOperatorControl() const;

		void StartCompetition();

		virtual void RobotInit()
		{
		}

		virtual void Disabled()
		{
		}

		virtual void Autonomous()
		{
		}

		virtual void OperatorControl()
		{
		}

		virtual void Test()
		{
		}

		virtual void RobotMain()
		{
		}

	protected:
		void setEnabled(bool en)
		{
			m_enabled = en;
		}


	private:
		bool ProcessCmdLineArgs();
		void InternalControl();

	protected:
		enum class RobotMode
		{
			Autonomous,
			Operator,
			Test,
			Finished,
		};

		void setRobotMode(SampleRobot::RobotMode m)
		{
			m_mode = m;
		}
	private:
		bool m_running;
		bool m_enabled;
		bool m_robotMainOverridden;
		RobotMode m_mode;
		std::vector<std::string> m_args;
		std::thread m_controller;

		double m_start_delay;
		double m_auto_period;
		double m_teleop_period;

		bool m_auto_done;
	};
}

