#include "Robot2018Operator.h"

Robot2018Operator::Robot2018Operator(xerolib::XeroRobotBase &robot) : OperatorControllerBase(robot)
{
    std::shared_ptr<xerolib::SubsystemBase> sub_p;

    sub_p = robot.getSubsystem("drivebase");
    m_drive_p = std::dynamic_pointer_cast<xerolib::DriveBase>(sub_p);
}

void Robot2018Operator::processJoystick(int joy, const std::vector<int> &pov,
					const std::vector<double> &axis, const std::vector<bool> &buttons)
{
    
}


