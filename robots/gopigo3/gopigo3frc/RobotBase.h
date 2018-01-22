#pragma once

#include <vector>
#include <string>
#include <exception>
#include <iostream>
#include "GoPiGo3Robot.h"

#define START_ROBOT_CLASS(_ClassName_)                                               \
  int main(int ac, char **av) {                                                      \
    _ClassName_ *robot = new _ClassName_();                                          \
	while (ac-- > 0) robot->addArg(*av++) ;			         						 \
	try {									         								 \
        frc::RobotBase::robotSetup(robot);                                           \
    }                                                                                \
    catch (const std::exception &ex)                                                 \
    {																		         \
        std::cout << "exception caught from robot - " << ex.what() << std::endl ;    \
    }                                                                                \
    return 0;                                                                        \
  }

namespace frc
{
	class RobotBase
	{
	public:
		RobotBase();
		virtual ~RobotBase();

		bool IsDisabled() const;
		bool IsAutonomous() const;
		bool IsEnabled() const;
		bool IsTest() const;
		bool IsOperatorControl() const;

		virtual void StartCompetition() = 0;

		static void robotSetup(RobotBase *robot);

		static gopigo3::GoPiGo3Robot &getRobotHardware() 
		{
			return m_robot_base_p->m_hardware;
		}

		void addArg(const char *arg_p)
		{
			m_args.push_back(arg_p);
		}

	protected:
		enum class RobotMode
		{
			Disabled,
			Autonomous,
			Operator,
			Test,
			Finished,
		};

		void setRobotMode(RobotMode mode);
		void stopAll()
		{
			m_hardware.reset();
		}

		const std::vector<std::string> &getArgs()
		{
			return m_args;
		}

	private:
		//
		// The current robot mode
		//
		RobotMode m_mode;

		//
		// The GoPiGo3 robot hardware
		//
		gopigo3::GoPiGo3Robot m_hardware;

		//
		// The one and only robot base
		//
		static RobotBase *m_robot_base_p;

		//
		// The command line args
		//
		std::vector<std::string> m_args;
	};
}

