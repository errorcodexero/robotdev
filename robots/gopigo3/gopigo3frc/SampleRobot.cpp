#include "SampleRobot.h"
#include <chrono>
#include <iostream>

namespace frc
{
	SampleRobot::SampleRobot() {
		m_robotMainOverridden = true;
		m_start_delay = 1.0;
		m_auto_period = 15.0;
		m_teleop_period = 0.0;
	}

	SampleRobot::~SampleRobot() {
	}

	void SampleRobot::RobotMain() {
		m_robotMainOverridden = false;
	}

	void SampleRobot::RobotInit() {
	}

	void SampleRobot::Disabled() {
		std::cout << "Robot Mode - disabled" << std::endl;
	}

	void SampleRobot::Autonomous() {
		std::cout << "Robot Mode - autonomous" << std::endl;
	}

	void SampleRobot::Test() {
	}

	void SampleRobot::OperatorControl() {
	}

	void SampleRobot::InternalControl()
	{
		int ms;

		setRobotMode(RobotBase::RobotMode::Disabled);
		ms = static_cast<int>(m_start_delay * 1000);
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));

		setRobotMode(RobotBase::RobotMode::Autonomous);
		ms = static_cast<int>(m_auto_period * 1000);
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));

		setRobotMode(RobotBase::RobotMode::Operator);
		ms = static_cast<int>(m_teleop_period * 1000);
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));

		setRobotMode(RobotBase::RobotMode::Disabled);
		stopAll();

		m_running = false;
		setRobotMode(RobotBase::RobotMode::Finished);
	}

	void SampleRobot::DriverStationControl()
	{
		setRobotMode(RobotBase::RobotMode::Disabled);

		while (true)
		{
		}
	}

	bool SampleRobot::ProcessCmdLineArgs()
	{
		bool ret = true;
		const std::vector<std::string> &args = getArgs();
		size_t index = 1;

		while (index < args.size())
		{
			if (args[index] == "--start" || args[index] == "--auto" || args[index] == "--oper")
			{
				index++;
				if (index == args.size())
				{
					std::cout << "SampleRobot: argument '--start' requires an argument" << std::endl;
					ret = false;
					break;
				}

				double value;
				size_t end;

				try
				{
					value = std::stod(args[index], &end);
					if (end != args[index].length())
					{
						std::cout << "SampleRobot: invalid value '" << args[index];
						std::cout << "' for argument '" << args[index - 1] << "'" << std::endl;
						ret = false;
						break;
					}

					if (args[index - 1] == "--start")
						m_start_delay = value;
					else if (args[index - 1] == "--auto")
						m_auto_period = value;
					else if (args[index - 1] == "--oper")
						m_teleop_period = value;
				}
				catch (const std::exception &ex)
				{
					std::cout << "SampleRobot: invalid value '" << args[index];
					std::cout << "' for argument '" << args[index - 1] << "'" << std::endl;
					ret = false;
					break;
				}

				index++;
			}
			else if (args[index] == "--station")
			{
				m_station = true;
			}
			else
			{
				std::cout << "SampleRobot: invalid command line argument '";
				std::cout << args[index] << "' - robot aborting" << std::endl;
				std::cout << "Valid Arguments" << std::endl;
				std::cout << "  --start NUMBER" << std::endl;
				std::cout << "  --auto NUMBER" << std::endl;
				std::cout << "  --oper NUMBER" << std::endl;
				ret = false;
				break;
			}
		}

		return ret;
	}

	void SampleRobot::StartCompetition() 
	{
		//
		// Process command line arguments
		//
		if (!ProcessCmdLineArgs())
			return;

		//
		// Initialize the robot hardware
		//
		RobotInit();

		//
		// Allow a derived class to take over the main loop
		//
		RobotMain();

		//
		// Start the thread that manages the competition
		//
		if (m_station)
			m_controller = std::thread(&SampleRobot::DriverStationControl, this);
		else
			m_controller = std::thread(&SampleRobot::InternalControl, this);

		//
		// If the main loop was not taken over, supply a main loop where
		// we ask the robot to handle specific modes
		//
		if (!m_robotMainOverridden) {
			m_running = true;
			while (m_running) {
				if (IsDisabled()) {
					Disabled();
					while (IsDisabled());
				}
				else if (IsAutonomous()) {
					Autonomous();
					while (IsAutonomous() && IsEnabled());
				}
				else if (IsTest()) {
					Test();
					while (IsTest() && IsEnabled());
				}
				else {
					OperatorControl();
					while (IsOperatorControl() && IsEnabled());
				}
			}

			std::cout << "\r\n ************ Robot program ending ***********" << std::endl;
		}
	}
}
