#include "Grabber.h"
#include "ParamsParser.h"
#include <XeroRobotBase.h>
#include <cmath>
#include <algorithm>
#include <numeric>

Grabber::Grabber(xerolib::XeroRobotBase &robot,
				 std::shared_ptr<frc::SpeedController> motor_p,
				 std::shared_ptr<frc::Encoder> encoder_p,
				 size_t nSamples) : xerolib::SubsystemBase("grabber", robot)
{
	xerolib::ParamsParser &params = xerolib::ParamsParser::get() ;

	m_motor_p = motor_p ;
	m_encoder_p = encoder_p ;
	
	m_open_angle = params.getValue("grabber:open") ;
	m_closed_angle = params.getValue("grabber::closed") ;
	m_stowed_angle = params.getValue("grabber::stowed") ;
	m_folded_angle = params.getValue("grabber::folded") ;

	m_holding_voltage = params.getValue("grabber::holding_voltage") ;
	m_grasping_voltage = params.getValue("grabber::grasping_voltage") ;

	m_stationary_threshold = static_cast<int32_t>(params.getValue("grebber::stationary_threshold") + 0.5) ;

	double p = params.getValue("grabber:p") ;
	double i = params.getValue("grabber:i") ;
	double d = params.getValue("grabber:d") ;
	double f = params.getValue("grabber:f") ;
	double imax = params.getValue("grabber:imax") ;
	double vmin = params.getValue("grabber:vmin") ;
	double vmax = params.getValue("grabber:vmax") ;
	
	m_pid.Init(p, i, d, f, vmin, vmax, imax) ;

	m_last_voltage = std::nan("") ;
	m_last_time = std::nan("") ;

	m_samples_taken = 0 ;
	m_samples.resize(nSamples) ;

	m_calibrated = false ;

	m_angle = std::nan("") ;

}

Grabber::~Grabber()
{
}

void Grabber::getInputs()
{
	int32_t sample = m_encoder_p->Get() ;
		
	if (m_samples_taken == m_samples.size())
	{
		for(size_t i = 1 ; i < m_samples.size() ; i++)
			m_samples[i - 1] = m_samples[i] ;
		m_samples[m_samples.size() - 1] = sample ;
	}
	else
	{
		m_samples[m_samples_taken] = sample ;
	}

	if (m_calibrated)
		m_angle = (sample - m_encoder_base) * kDegreesPerTick ;
}

void Grabber::setOutputs()
{
	switch(m_state)
	{
	case State::Idle:
		setMotorVoltage(0.0) ;
		break ;
	case State::Calibrate:
		doCalibration() ;
		break ;
	case State::Hold:
		setMotorVoltage(m_holding_voltage) ;
		break ;
	case State::Grasp:
		doGrasp() ;
		break ;
	case State::Angle:
		doAngle() ;
		break ;
	}
}

bool Grabber::isStationary()
{
	int32_t minsamp = *std::min_element(m_samples.begin(), m_samples.end()) ;
	int32_t maxsamp = *std::max_element(m_samples.begin(), m_samples.end()) ;
	return maxsamp - minsamp < m_stationary_threshold ;
}

void Grabber::setMotorVoltage(double v)
{
	if (std::isnan(m_last_voltage) || std::fabs(m_last_voltage - v) > kDelta)
	{
		m_motor_p->Set(v) ;
		m_last_voltage = v ;
	}
}

void Grabber::doCalibration()
{
	if (isStationary())
	{
		m_calibrated = true ;
		m_state = State::Hold ;
		m_encoder_base = std::accumulate(m_samples.begin(), m_samples.end(), 0) / m_samples.size() ;
	}
}

void Grabber::doGrasp()
{
	setMotorVoltage(m_grasping_voltage) ;
	
	if (isStationary())
	{
		if (getCurrentAngle() < m_folded_angle)
		{
			m_state = State::Idle ;
		}
		else
		{
			m_state = State::Hold ;
		}
	}
}

void Grabber::doAngle()
{
	xerolib::XeroRobotBase &robot = getRobot() ;
	double now = robot.getTime() ;

	if (!std::isnan(m_last_time))
	{
		double dt = now - m_last_time ;
		double out = m_pid.getOutput(m_target, m_angle, dt) ;

		setMotorVoltage(out) ;
	}
	m_last_time = now ;
}
