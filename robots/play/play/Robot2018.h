#pragma once

#include "XeroRobotBase.h"
#include "DriveBase.h"
#include "Robot2018Autonomous.h"
#include <map>
#include <string>

class Robot2018 : public xerolib::XeroRobotBase
{
public:
    Robot2018();
    virtual ~Robot2018() ;

    virtual void RobotInit();

protected:
    /// @brief this method is implemented by a derived class and creates the autonomous controller
    virtual std::shared_ptr<xerolib::RobotControllerBase> createAutonomousController()
    {
		std::shared_ptr<Robot2018Autonomous> ctrl_p = std::make_shared<Robot2018Autonomous>(*this);
		return ctrl_p;
    }

    /// @brief this method is implemented by a derived class and creates the operator controller
    virtual std::shared_ptr<xerolib::RobotControllerBase> createOperatorControlController()
    {
		return nullptr ;
    }

    /// @brief this method is implemented by a derived class and creates the test controller
    virtual std::shared_ptr<xerolib::RobotControllerBase> createTestController()
    {
		return nullptr;
    }

private:
	void createDriveBase() ;
	void createLiftingCollector() ;
	void createWings() ;

private:
    //
    // The drive base for this robot
    //
    std::shared_ptr<xerolib::DriveBase> m_drivebase_p;
};
