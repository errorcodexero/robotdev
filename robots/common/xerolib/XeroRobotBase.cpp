#include "XeroRobotBase.h"
#include "RobotControllerBase.h"
#include "SubsystemBase.h"
#include "Timer.h"
#include "DataUdpDestination.h"
#include "DataTcpDestination.h"
#include "DataDashboardDestination.h"
#include "ParamFileReader.h"
#include <iostream>

namespace xerolib
{
    XeroRobotBase::XeroRobotBase()
    {
	m_looptime = 0.05;
    }

    XeroRobotBase::~XeroRobotBase()
    {
	m_data.endDataSet();
    }

    void XeroRobotBase::setupConsoleLogger()
    {
	m_logger.addStandardOutputDestination();
    }

    void XeroRobotBase::readParams()
    {
	xerolib::MessageLogger &logger = getMessageLogger();

	std::string file = getenv("HOME");
	file += "/params";

	if (!xerolib::ParamFileReader::readParamFile(logger, file, m_parameters))
	{
	    logger << xerolib::MessageLogger::MessageType::Debug << "cannot read parameter file '";
	    logger << file << "'" << xerolib::MessageLogger::Token::EndOfMessage;
	}
    }

    double XeroRobotBase::getParam(const std::string &name, double defvalue)
    {
	auto it = m_parameters.find(name);
	if (it == m_parameters.end())
	    return defvalue;

	return it->second;
    }

    void XeroRobotBase::setupLoggers(const char *msgname_p, const char *dataname_p)
    {
	std::string filename;

	filename = getenv("HOME");
	filename += "/";
	filename += msgname_p;
	m_logger.addDestination(filename);

	filename = getenv("HOME");
	filename += "/";
	filename += dataname_p;
	m_data.addDestination(filename);

	m_time_col = m_data.createColumn("time");
	m_mode_col = m_data.createColumn("mode");
    }

    void XeroRobotBase::setupUdpLogger(int port)
    {
	std::shared_ptr<DataDestination> dest_p = std::make_shared<DataUdpDestination>(port);
	m_data.addDestination(dest_p);
    }

    void XeroRobotBase::setupTcpLogger(const std::string &addr, uint16_t port)
    {
	std::shared_ptr<DataDestination> dest_p = std::make_shared<DataTcpDestination>(addr, port);
	m_data.addDestination(dest_p);
    }

    void XeroRobotBase::setupSmartDashboardLogger()
    {
	std::shared_ptr<DataDashboardDestination> dest_p = std::make_shared<DataDashboardDestination>() ;
	m_data.addDestination(dest_p) ;
    }

    void XeroRobotBase::Disabled()
    {
	getMessageLogger() << MessageLogger::MessageType::Info;
	getMessageLogger() << "Entering mode: Disabled" << MessageLogger::Token::EndOfMessage;

	while (!IsEnabled())
	{
	}
    }

    void XeroRobotBase::Autonomous()
    {
	getMessageLogger() << MessageLogger::MessageType::Info;
	getMessageLogger() << "Entering mode: Autonomous" << MessageLogger::Token::EndOfMessage;

	m_controller_p = createAutonomousController();

	while (IsAutonomous() && IsEnabled())
	{
	    doOneLoop(RobotModeType::AutonomousMode);
	}
    }

    void XeroRobotBase::OperatorControl()
    {
	getMessageLogger() << MessageLogger::MessageType::Info;
	getMessageLogger() << "Entering mode: Operator" << MessageLogger::Token::EndOfMessage;

	m_controller_p = createOperatorControlController();

	while (IsOperatorControl() && IsEnabled())
	{
	    doOneLoop(RobotModeType::OperatorMode);
	}
    }

    void XeroRobotBase::Test()
    {
	getMessageLogger() << MessageLogger::MessageType::Info;
	getMessageLogger() << "Entering mode: Test" << MessageLogger::Token::EndOfMessage;

	m_controller_p = createTestController();

	while (IsTest() && IsEnabled())
	{
	    doOneLoop(RobotModeType::TestMode);
	}
    }

    void XeroRobotBase::doOneLoop(RobotModeType mode)
    {
	double start = getTime();

	m_data.startRow();
	m_data.logData(m_time_col, start);
	m_data.logData(m_mode_col, mode);

	for (auto sub_p : m_subsystems)
	    sub_p->readInputs();

	if (m_controller_p != nullptr)
	    m_controller_p->executeOneLoop();

	for (auto sub_p : m_subsystems)
	    sub_p->setOutputs();

	double elapsed = getTime() - start;

	if (elapsed < m_looptime)
	{
	    frc::Wait(m_looptime - elapsed);
	}
	else
	{
	    getMessageLogger() << MessageLogger::MessageType::Warning;
	    getMessageLogger() << "robot exceeded the target loop time, target " << m_looptime;
	    getMessageLogger() << ", actual " << elapsed << MessageLogger::Token::EndOfMessage;
	}

	m_data.endRow();
    }

    std::shared_ptr<SubsystemBase> XeroRobotBase::getSubsystem(const char *name_p)
    {
	for (auto sub_p : m_subsystems)
	{
	    if (sub_p->getName() == name_p)
		return sub_p;
	}

	return nullptr;
    }
}
