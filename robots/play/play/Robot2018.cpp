#include "Robot2018.h"
#include "MessageLogger.h"
#include "ParamsParser.h"
#include <AHRS.h>

#include <ctre/phoenix/MotorControl/CAN/WPI_TalonSRX.h>
#include <Encoder.h>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace xerolib;


Robot2018::Robot2018()
{
}

Robot2018::~Robot2018()
{
}

void Robot2018::RobotInit()
{
    setupConsoleLogger();
    readParams("/home/lvuser/params.txt");

	createDriveBase() ;
	createLiftingCollector() ;
	createWings() ;
}

void Robot2018::createDriveBase()
{
	xerolib::ParamsParser &params = xerolib::ParamsParser::get() ;
    std::list<std::shared_ptr<frc::SpeedController>> left_motors ;
    std::list<std::shared_ptr<frc::SpeedController>> right_motors ;
    
    std::list<int> left_can_addrs = { 1, 2, 3} ;
    std::list<int> right_can_addrs = { 4, 5, 6} ;

	
    for(int addr: left_can_addrs)
    {
		std::shared_ptr<frc::SpeedController> motor_p = std::make_shared<ctre::phoenix::motorcontrol::can::WPI_TalonSRX>(addr) ;
		motor_p->SetInverted(true) ;
		left_motors.push_back(motor_p) ;
    }
    
    for(int addr: right_can_addrs)
    {
		std::shared_ptr<frc::SpeedController> motor_p = std::make_shared<ctre::phoenix::motorcontrol::can::WPI_TalonSRX>(addr) ;
		motor_p->SetInverted(true) ;
		right_motors.push_back(motor_p) ;
    }
	
    std::shared_ptr<frc::Encoder> left_enc_p = std::make_shared<frc::Encoder>(0, 1);
    std::shared_ptr<frc::Encoder> right_enc_p = std::make_shared<frc::Encoder>(2, 3);
	std::shared_ptr<AHRS> navx_p = std::make_shared<AHRS>(frc::SPI::Port::kMXP) ;
    m_drivebase_p = std::make_shared<DriveBase>(*this, left_motors, right_motors, left_enc_p, right_enc_p, navx_p) ;
	int32_t ticks =  static_cast<int32_t>(params.getValue("drivebase:encoder_ticks")) ;
	double diameter = params.getValue("drivebase:diameter") ;
	double width = params.getValue("drivebase:width") ;
	double scrub = params.getValue("drivebase:scrub") ;
	
    m_drivebase_p->setPhysicalChar(ticks, diameter, width, scrub) ;

    addSubsystem(m_drivebase_p);
}

void Robot2018::createLiftingCollector()
{
}

void Robot2018::createWings()
{
}


START_ROBOT_CLASS(Robot2018);
