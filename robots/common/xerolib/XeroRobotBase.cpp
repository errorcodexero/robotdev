#include "XeroRobotBase.h"
#include "RobotControllerBase.h"
#include "SubsystemBase.h"
#include "Timer.h"
#include "ParamsParser.h"
#include <iostream>

namespace xerolib
{
    XeroRobotBase::XeroRobotBase()
    {
		m_looptime = 0.02;
    }

    XeroRobotBase::~XeroRobotBase()
    {
    }

    void XeroRobotBase::setupConsoleLogger()
    {
		m_logger.addStandardOutputDestination();
    }

    void XeroRobotBase::readParams(const std::string &file)
    {
		std::string filename = file;

		if (filename.length() == 0)
			filename = "/home/lvuser/params.txt" ;

		xerolib::MessageLogger &logger = getMessageLogger();

		ParamsParser &params = ParamsParser::get();
		if (!params.readFile(file))
		{
			logger << xerolib::MessageLogger::MessageType::Debug << "cannot read parameter file '";
			logger << file << "'" << xerolib::MessageLogger::Token::EndOfMessage;
		}
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

		for (auto sub_p : m_subsystems)
			sub_p->getInputs();

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
