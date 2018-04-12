#include <XeroRobotBase.h>
#include "Collector.h"
#include "Intake.h"
#include "Grabber.h"
#include "ParamsParser.h"

Collector::Collector(xerolib::XeroRobotBase &robot,
					 std::shared_ptr<Intake> intake_p,
					 std::shared_ptr<Grabber> grabber_p,
					 std::shared_ptr<DigitalInput> detect_p)
	: xerolib::SubsystemBase("collector", robot)
{
	m_intake_p = intake_p ;
	m_grabber_p = grabber_p ;
	m_cube_detect_p = detect_p ;
	m_state = State::IDLE ;

	xerolib::ParamsParser &params = xerolib::ParamsParser::get() ;
	m_default_collect_speed = params.getValue("collector:collect_speed") ;
	m_default_drop_speed = params.getValue("collector:drop_speed") ;
	m_default_drop_time = params.getValue("collector:drop_time") ;
	m_default_eject_speed = params.getValue("collector:eject_speed") ;
	m_default_eject_time = params.getValue("collector:eject_time") ;
	m_maybe_has_cube_time = params.getValue("collector:maybe_has_cube_time") ;
	m_maybe_lost_cube_time = params.getValue("collector:maybe_lost_cube_time") ;
	m_pulse_intake_speed = params.getValue("collector::pulse_intake_speed") ;
}

Collector::~Collector()
{
}

void Collector::calibrate()
{
	m_grabber_p->calibrate() ;
}

void Collector::eject(double speed, double etime)
{
	if (!m_grabber_p->isCalibrated())
		return ;

	m_state = State::EJECTING ;
	m_cube_state = CubeStatus::NoCube ;
	m_grabber_p->gotoAngle(Grabber::Preset::CLOSED) ;
	m_intake_p->run(speed, etime) ;
}

void Collector::drop(double speed, double etime)
{	
	if (!m_grabber_p->isCalibrated())
		return ;

	m_state = State::DROPPING ;
	m_grabber_p->gotoAngle(Grabber::Preset::OPEN) ;
	m_intake_p->run(speed, etime) ;
}

void Collector::openCollect(double speed)
{
	if (!m_grabber_p->isCalibrated())
		return ;

	m_state = State::COLLECTING ;
	m_grabber_p->gotoAngle(Grabber::Preset::OPEN) ;
	m_intake_p->run(speed) ;
}

void Collector::closeCollect(double speed)
{
	if (!m_grabber_p->isCalibrated())
		return ;

	m_state = State::COLLECTING ;
	m_grabber_p->gotoAngle(Grabber::Preset::OPEN) ;
	m_intake_p->run(speed) ;
}	

void Collector::collect(double angle, double speed)
{
	if (!m_grabber_p->isCalibrated())
		return ;

	m_state = State::COLLECTING ;
	m_grabber_p->gotoAngle(angle) ;
	m_intake_p->run(speed) ;
}

void Collector::getInputs()
{
	xerolib::XeroRobotBase &robot = getRobot() ;
	double now = robot.getTime() ;
	bool sensor = !m_cube_detect_p->Get() ;
	
	switch(m_cube_state)
	{
	case CubeStatus::NoCube:
		if (sensor && !m_grabber_p->isFolded() && (m_state == State::COLLECTING || m_state == State::IDLE))
		{
			//
			// If we are collecting or idle, and the cube sensor detects a cube
			// start the sequence of acquiringthe cube
			//
			m_cube_timer.set(now, m_maybe_has_cube_time) ;
			m_cube_state = CubeStatus::MaybeHasCube ;
		}
		break ;
	case CubeStatus::MaybeHasCube:
		if (!sensor)
		{
			//
			// The sensor turned off in the detect window, it was just a glitch
			//
			m_cube_state = CubeStatus::NoCube ;
		}
		else if (m_cube_timer.done(now))
		{
			//
			// The sensor is still on, grasp on the cube
			//
			m_cube_state = CubeStatus::GraspCube ;
			m_grabber_p->grasp() ;
		}
		break ;
	case CubeStatus::GraspCube:
		if (m_grabber_p->isIdle())
		{
			m_cube_state = CubeStatus::NoCube ;
		}
		else if (m_grabber_p->isHolding())
		{
			pulseIntakeInit(now) ;
			m_cube_state = CubeStatus::HasCube ;
		}
		break ;
	case CubeStatus::HasCube:
		if (!sensor)
		{
			m_cube_timer.set(now, m_maybe_lost_cube_time) ;
			m_cube_state = CubeStatus::MaybeLostCube ;
		}
		else
		{
			pulseIntake(now) ;
		}
		break ;
	case CubeStatus::MaybeLostCube:
		if (sensor)
		{
			m_cube_state = CubeStatus::HasCube ;
		}
		else if (m_cube_timer.done(now))
		{
			m_cube_state = CubeStatus::NoCube ;
		}
		break ;
	}
}

void Collector::setOutputs()
{
	if (m_state == State::DROPPING || m_state == State::EJECTING)
	{
		if (!m_intake_p->isRunning())
		{
			m_state = State::IDLE ;
			m_grabber_p->idle() ;
		}
	}
}

void Collector::pulseIntakeInit(double now)
{
	m_pulse_intake_timer.set(now, m_has_cube_intake_run_interval) ;
}

void Collector::pulseIntake(double now)
{
	if (m_pulse_intake_timer.done(now))
	{
		m_pulse_intake_timer.set(now, m_has_cube_intake_run_interval) ;
		m_intake_p->run(m_pulse_intake_speed, m_has_cube_intake_run_duration) ;
	}
}
