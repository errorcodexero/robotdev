#pragma once

#include "MessageLogger.h"
#include "FileManager.h"
#include <Timer.h>
#include <SampleRobot.h>
#include <memory>
#include <list>
#include <map>

namespace xerolib
{
    class RobotControllerBase;
    class SubsystemBase;

    class XeroRobotBase : public frc::SampleRobot
    {
    public:
	XeroRobotBase();
	virtual ~XeroRobotBase();

	void setupConsoleLogger() ;

	/// @brief called when the robot is disabled
	virtual void Disabled();

	/// @brief called when the robot is operating in autonomous mode
	virtual void Autonomous();

	/// @brief called when the robot is under operator control
	virtual void OperatorControl();

	/// @brief called when the robot is in test mode
	virtual void Test();

	/// @brief called to get the current robot time in seconds
	double getTime()
	{
	    return frc::Timer::GetFPGATimestamp();
	}

	/// @brief return the message logger for the robot
	/// @returns the message logger for the robot
	MessageLogger &getMessageLogger()
	{
	    return m_logger;
	}

	/// @brief return the subsystem with the given name
	/// @param name the name of the subsystem to find
	/// @returns the subsystme with the given name, or nullptr if no subsystem with the name exists
	std::shared_ptr<SubsystemBase> getSubsystem(const char *name_p);

    protected:
	/// @brief add a new subsystem to the roboto
	/// @param subsystem_p the subsystem to add to the robot
	void addSubsystem(std::shared_ptr<SubsystemBase> subsystem_p)
	{
	    m_subsystems.push_back(subsystem_p);
	}

	/// @brief this method is implemented by a derived class and creates the autonomous controller
	virtual std::shared_ptr<RobotControllerBase> createAutonomousController() = 0;

	/// @brief this method is implemented by a derived class and creates the operator controller
	virtual std::shared_ptr<RobotControllerBase> createOperatorControlController() = 0;

	/// @brief this method is implemented by a derived class and creates the test controller
	virtual std::shared_ptr<RobotControllerBase> createTestController() = 0;

	/// @brief read the parameters file
	virtual void readParams(const std::string &filename);

	enum RobotModeType
	{
	    DisabledMode = 0,
	    AutonomousMode = 1,
	    OperatorMode = 2,
	    TestMode = 3,
	};

    private:

	//
	// Run a single robot loop
	//
	virtual void doOneLoop(RobotModeType mode);

    private:
	//
	// The controller controlling the robot
	//
	std::shared_ptr<RobotControllerBase> m_controller_p;

	//
	// The list of subsystems
	//
	std::list<std::shared_ptr<SubsystemBase>> m_subsystems;

	//
	// The time for a single robot loop
	//
	double m_looptime;

	//
	// The message logger for the robot
	//
	MessageLogger m_logger;

	//
	// The file manager for finding a location for the data and message files
	//
	FileManager m_filemgr ;

	//
	// The index for the time variable
	//
	size_t m_time_col;

	//
	// The mode of the robot (auto, disabled, oper, or test)
	//
	size_t m_mode_col;

    };
}

