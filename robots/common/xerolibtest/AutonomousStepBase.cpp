#include "AutonomousStepBase.h"
#include "AutonomousControllerBase.h"

namespace xerolib
{
	AutonomousStepBase::AutonomousStepBase(AutonomousControllerBase &controller) : m_controller(controller)
	{
	}

	AutonomousStepBase::~AutonomousStepBase()
	{
	}

	XeroRobotBase &AutonomousStepBase::getRobot()
	{
		return m_controller.getRobot();
	}
}