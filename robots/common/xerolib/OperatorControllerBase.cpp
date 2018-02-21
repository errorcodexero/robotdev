#include "OperatorControllerBase.h"
#include <DriverStation.h>

namespace xerolib
{
	OperatorControllerBase::OperatorControllerBase(XeroRobotBase &robot) : RobotControllerBase(robot)
	{
		m_joystick_count = 2;
	}

	OperatorControllerBase::~OperatorControllerBase()
	{
	}

	void OperatorControllerBase::executeOneLoop()
	{
#ifdef NOTYET
		DriverStation &ds = DriverStation::GetInstance();

		if (!ds.IsDSAttached())
			return;

		if (!ds.IsNewControlData())
			return;

		std::vector<int> pov;
		std::vector<double> axis;
		std::vector<bool> buttons;

		for (int joy = 0; joy < m_joystick_count; joy++)
		{
			pov.clear();
			axis.clear();
			buttons.clear();

			for (int index = 0; index < ds.GetStickPOVCount(joy); index++)
				pov.push_back(ds.GetStickPOV(joy, index));

			for (int index = 0; index < ds.GetStickAxisCount(joy); index++)
				axis.push_back(ds.GetStickAxis(joy, index));

			for (int index = 0; index < ds.GetStickButtonCount(joy); index++)
				buttons.push_back(ds.GetStickButton(joy, index));

			processJoystick(joy, pov, axis, buttons);
		}
#endif
	}
}
