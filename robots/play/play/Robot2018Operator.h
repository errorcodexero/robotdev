#pragma once

#include <OperatorControllerBase.h>
#include <DriveBase.h>

class Robot2018Operator : public xerolib::OperatorControllerBase
{
public:
    Robot2018Operator(xerolib::XeroRobotBase &robot) ;

protected:
    virtual void processJoystick(int joy, const std::vector<int> &pov,
				 const std::vector<double> &axis, const std::vector<bool> &buttons);

private:
    std::shared_ptr<xerolib::DriveBase> m_drive_p ;
} ;



