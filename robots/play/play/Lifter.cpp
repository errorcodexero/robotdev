#include "Lifter.h"
#include <ParamsParser.h>
#include <algorithm>

Lifter::Lifter(xerolib::XeroRobotBase &robot,
			   std::shared_ptr<frc::SpeedController> motor1_p,
			   std::shared_ptr<frc::SpeedController> motor2_p,
			   std::shared_ptr<frc::Encoder> encoder_p,
			   std::shared_ptr<frc::DigitalInput> lower_limit_p,
			   std::shared_ptr<frc::DigitalInput> upper_limit_p,
			   std::shared_ptr<frc::Solenoid> brake_p,
			   std::shared_ptr<frc::Solenoid> shifter_p,
			   bool quick, size_t nSamples)
	: xerolib::SubsystemBase("lifter", robot)
{
	xerolib::ParamsParser &params = xerolib::ParamsParser::get() ;
	double v ;
	
	m_motors.push_back(motor1_p) ;
	m_motors.push_back(motor2_p) ;
	m_encoder_p = encoder_p ;
	m_lower_limit_p = lower_limit_p ;
	m_upper_limit_p = upper_limit_p ;
	m_brake_p = brake_p ;
	m_shifter_p = shifter_p ;
	
	m_quick_calibrate = quick ;

	m_samples.resize(nSamples) ;
	m_samples_taken = 0 ;

	m_calibrated = false ;
	m_climb_baseline_set = false ;
	m_state = State::UNINITIALIZED ;
	m_shifter = ShifterState::UNKNOWN ;
	m_brake = BrakeState::UNKNOWN ;

	v = params.getValue("lifter:height:floor") ;
	m_height_map[Preset::FLOOR] = v ;
	v = params.getValue("lifter:height:exchange") ;
	m_height_map[Preset::EXCHANGE] = v ;
	v = params.getValue("lifter:height:switch") ;
	m_height_map[Preset::SWITCH] = v ;
	v = params.getValue("lifter:height:scalelow") ;
	m_height_map[Preset::SCALELOW] = v ;
	v = params.getValue("lifter:height:scalehigh") ;
	m_height_map[Preset::SCALEHIGH] = v ;
	v = params.getValue("lifter:height:climb") ;
	m_height_map[Preset::CLIMB] = v ;

	m_stationary_threshold = static_cast<int32_t>(params.getValue("lifter::stationary_threshold") + 0.5) ;
}

Lifter::~Lifter()
{
}

void Lifter::getInputs()
{
	int32_t ticks = m_encoder_p->Get() ;
	if (m_samples_taken == m_samples_size())
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
		m_actual_height = (ticks - m_ticks_base) * INCHES_PER_TICK_HIGH_GEAR + COLLECTOR_BOTTOM_OFFSET ;
}

void Lifter::setOutputs()
{
	switch(m_state)
	{
	case State::UNINITIALIZED:
		setMotors(0.0) ;
		brakeOn() ;
		highGear() ;
		break ;
	case State::CALIBRATING:
		doCalibrate() ;
		break ;
	case State::SEEKING:
		doSeek() ;
		break ;
	case State::HOLDING:
		doHold() ;
		break ;
	case State::SUSTAIN:
		doSustain() ;
		break ;
	case State::UP:
		doUp() ;
		break ;
	case State::DOWN:
		doDown() ;
		break ;
	}
}

void Lifter::setMotors(double v)
{
	for(auto motor_p : m_motors)
		motor_p->Set(v) ;
}

void Lifter::doCalibrate()
{
	brakeOff() ;
	highGear() ;

	if (m_quick_calibrate)
	{
		m_calibrated = true ;
		m_ticks_base = m_encoder_p->Get() ;
		m_state = State::HOLDING ;
	}
	else
	{
		if (isStationary())
		{
			m_ticks_base = std::accumulate(m_samples.begin(), m_samples.end(), 0) / m_samples.size() ;
			m_calibrated = true ;
			m_state = State::HOLDING ;
		}
		else
		{
			if (isInLowerLimit())
			{
			}
		}
	}
}

void Lifter::doSeek()
{
}

void Lifter::doHold()
{
	setMotors(0.0) ;
	brakeOn() ;
}

void Lifter::doSustain()
{
	if (!m_climb_baseline_set)
		return ;
}

void Lifter::gotoHeight(double height)
{
	if (m_state == State::UNINITIALIZED || m_state == State::CALIBRATING)
		return ;

	m_target_height = height ;
	m_state = State::SEEKING ;
}

bool Lifter::isStationary()
{
	int32_t minsamp = *std::min_element(m_samples.begin(), m_samples.end()) ;
	int32_t maxsamp = *std::max_element(m_samples.begin(), m_samples.end()) ;
	return maxsamp - minsamp < m_stationary_threshold ;
}

	
