#pragma once

#include "MessageLogger.h"
#include "DataLogger.h"
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

	/// @brief return the data logger for the robot
	/// @returns the data logger for the robot
	DataLogger &getDataLogger()
	{
	    return m_data;
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

	/// @brief setup the message log file and data file to the home directory of the robot user
	/// @param msgname_p the name of the file for messages
	/// @param dataname_p the name of the file for data
	virtual void setupLoggers(const char *msgname_p, const char *dataname_p);

	/// @brief setup for output message on the console
	virtual void setupConsoleLogger();

	/// @brief setup the UDP logger for the robot
	/// @param port the port number to broadcast data to
	virtual void setupUdpLogger(int port);

	/// @brief setup the TCP logger for the robot
	/// @param addr the address of the machine to connect to
	/// @param port the port number to connect to 
	virtual void setupTcpLogger(const std::string &addr, uint16_t port);

	/// @brief setup the logger to send data to the FRC smart dashboard
	virtual void setupSmartDashboardLogger() ;

	/// @brief read the parameters file
	virtual void readParams();

	/// @brief return the value of a parameter, or the default value if the parameter was not in the file
	/// @param name the name of the parameter in the parameter file
	/// @param defvalue the default value of the parameter
	/// @returns the value of the parameter
	double getParam(const std::string &name, double defvalue);


    private:
	enum RobotModeType
	{
	    DisabledMode = 0,
	    AutonomousMode = 1,
	    OperatorMode = 2,
	    TestMode = 3,
	};

	//
	// Run a single robot loop
	//
	void doOneLoop(RobotModeType mode);

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
	// The data logger for the robot
	//
	DataLogger m_data;

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

	//
	// The map of params read from the params file
	//
	std::map<std::string, double> m_parameters;
    };
}

