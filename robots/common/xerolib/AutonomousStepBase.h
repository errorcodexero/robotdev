#pragma once

#include <string>

namespace xerolib
{
	class AutonomousControllerBase;
	class XeroRobotBase;

	class AutonomousStepBase
	{
	public:
		AutonomousStepBase(AutonomousControllerBase &controller);
		virtual ~AutonomousStepBase();


		virtual void start() = 0;
		virtual bool run() = 0;
		virtual void end() = 0;
		virtual std::string toString() = 0;

	protected:
		AutonomousControllerBase &getController()
		{
			return m_controller;
		}

		XeroRobotBase &getRobot();

	private:
		AutonomousControllerBase &m_controller;
	};
}

