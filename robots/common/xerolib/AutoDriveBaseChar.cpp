#include "AutoDriveBaseChar.h"
#include "DriveBase.h"
#include <sstream>

namespace xerolib
{
	AutoDriveBaseChar::AutoDriveBaseChar(AutonomousControllerBase &controller, std::shared_ptr<DriveBase> drive_p, double voltage, double dur, bool linear)
		: AutonomousStepBase(controller)
	{
		m_drive_base_p = drive_p;
		m_voltage = voltage;
		m_duration = dur;
		m_linear = linear;
	}

	AutoDriveBaseChar::~AutoDriveBaseChar()
	{
	}

	void AutoDriveBaseChar::start()
	{
		auto &robot = getRobot();
		m_next_time = robot.getTime() + 1;
		m_state = State::Waiting;
	}

	bool AutoDriveBaseChar::run()
	{
		bool ret = false;
		auto &robot = getRobot();
		auto &logger = robot.getMessageLogger();
		double time = robot.getTime();

		if (m_state == State::Waiting)
		{
			if (time > m_next_time)
			{
				m_drive_base_p->setMotorVoltage(m_voltage, m_voltage);
				m_next_time = time + 1.0;
				m_state = State::Running;
			}
		}
		else if (m_state == State::Running)
		{
			if (time > m_next_time)
			{
				if (m_linear)
					m_drive_base_p->startLinearMeasure();
				else
					m_drive_base_p->startRotationalMeasure();

				m_next_time = time + m_duration;
				m_state = State::Measuring;
			}
		}
		else if (m_state == State::Measuring)
		{
			if (time > m_next_time)
			{
				double speed;
				
				if (m_linear)
					speed = m_drive_base_p->endLinearMeasure();
				else
					speed = m_drive_base_p->endRotationalMeasure();
				logger << MessageLogger::MessageType::Debug;
				logger << "measure " << m_voltage << " " << speed;
				logger << MessageLogger::Token::EndOfMessage;
				ret = true;
			}
		}

		return ret;
	}

	void AutoDriveBaseChar::end()
	{
	}

	std::string AutoDriveBaseChar::toString()
	{
		std::stringstream strm;

		strm << "drive base characterization, voltage = " << m_voltage;
		strm << ", duration = " << m_duration;
		return strm.str();
	}
}
