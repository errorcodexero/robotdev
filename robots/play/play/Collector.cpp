#include "Collector.h"
#include "Intake.h"
#include "Gragger.h"

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
}

void Collector::eject(double speed, double etime)
{
	if (!m_grabber_p->isCalibrated())
		return ;

	m_state = State::EJECTING ;
	m_grabber_p->gotoAngle(Grabber::Preset::CLOSED) ;
	m_intake_p->run(speed, etime) ;
}

void Collector::drop(double speed, double eTime)
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
	m_intake_p->run(speed, etime) ;
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
	if (m_cube_detect_p->Get() == false)
	{
		//
		// The cube detector has detected a cube
		//
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
