#pragma once

#include "RobotControllerBase.h"
#include <vector>

namespace xerolib
{
    class OperatorControllerBase : public RobotControllerBase
    {
    public:
	OperatorControllerBase(XeroRobotBase &robot) ;
	virtual	~OperatorControllerBase() ;

	virtual void executeOneLoop() ;

    protected:
	void setJoystickCount(int count)
	{
	    m_joystick_count = count ;
	}

	virtual void processJoystick(int joy, const std::vector<int> &pov,
				     const std::vector<double> &axis, const std::vector<bool> &buttons) = 0 ;

    private:
	int m_joystick_count ;
    } ;
}
