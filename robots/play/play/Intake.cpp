#include "Intake.h"
#include <XeroRobotBase.h>
#include <cmath>
#include <limits>

Intake::Intake(xerolib::XeroRobotBase &base,
			   std::shared_ptr<frc::SpeedController> left_p,
			   std::shared_ptr<frc::SpeedController> right_p) : xerolib::SubsystemBase("Intake", base)
{
	m_last_value = std::nan("") ;
	m_left_p = left_p ;
	m_right_p = right_p ;
}

Intake::~Intake()
{
}

void Intake::getInputs()
{
}

void Intake::setOutputs()
{
	if (isRunning())
	{
		xerolib::XeroRobotBase &robot = getRobot() ;
		double now = robot.getTime() ;
		if (now > m_started + m_duration)
			writeMotors(0.0) ;
	}
}

void Intake::run(double speed)
{
	xerolib::XeroRobotBase &robot = getRobot() ;
	m_speed = speed ;
	m_started = robot.getTime() ;
	m_duration = std::numeric_limits<double>::max() ;
}

void Intake::run(double speed, double duration)
{
	xerolib::XeroRobotBase &robot = getRobot() ;
	m_speed = speed ;
	m_started = robot.getTime() ;
	m_duration = duration ;
}

bool Intake::isRunning() const
{
	return !std::isnan(m_last_value) && std::fabs(m_last_value) > kDelta ;
}

void Intake::writeMotors(double value)
{
	if (std::isnan(m_last_value) || std::fabs(m_last_value - value) > kDelta)
	{
		m_left_p->Set(value) ;
		m_right_p->Set(value) ;
		m_last_value = value ;
	}
}


		
