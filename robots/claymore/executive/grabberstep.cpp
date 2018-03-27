#include "grabberstep.h"

GrabberStep::GrabberStep(GrabberController::Preset target_preset)
{
	init = false ;
	preset = target_preset ;
	anglegiven = false ;
}

GrabberStep::GrabberStep(double a)
{
	init = false ;
	angle = a ;
	anglegiven = true ;
}

GrabberStep::GrabberStep(const char *param_p, double a)
{
	anglegiven = false ;
	param = param_p ;
	angle = a ;
	init = false ;
}

Step::Status GrabberStep::done(Next_mode_info info)
{
    Step::Status ret = Step::Status::UNFINISHED ;

	if (anglegiven && ready(status(info.status.grabber), Grabber::Goal::go_to_angle(angle)))
	{
		ret = Step::Status::FINISHED_SUCCESS ;
	}
	else if (param.length() > 0 && ready(status(info.status.grabber), Grabber::Goal::go_to_angle(angle)))
	{
		ret = Step::Status::FINISHED_SUCCESS ;
	}
	else if (!anglegiven && param.length() == 0 && ready(status(info.status.grabber), Grabber::Goal::go_to_preset(preset)))
	{
		ret = Step::Status::FINISHED_SUCCESS ;
	}

    if (ret == Step::Status::FINISHED_SUCCESS)
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Grabber To Preset complete" ;
		logger.endMessage() ;
    }
    return ret ;
}

Toplevel::Goal GrabberStep::run(Run_info info)
{
    return run(info,{});
}

Toplevel::Goal GrabberStep::run(Run_info info,Toplevel::Goal goals)
{
    if(!init)
	{
		if (anglegiven)
		{
			Grabber::grabber_controller.moveToAngle(angle, info.in.now) ;
		}
		else if (param.length() > 0)
		{
			paramsInput *params_p = paramsInput::get() ;
			angle = params_p->getValue(param, angle) ;
			Grabber::grabber_controller.moveToAngle(angle, info.in.now) ;
		}
		else
		{
			Grabber::grabber_controller.moveToAngle(preset, info.in.now) ;
		}
		
		init = false;
    }

	if (anglegiven || param.length() > 0)
		goals.grabber = Grabber::Goal::go_to_angle(angle) ;
	else
		goals.grabber = Grabber::Goal::go_to_preset(preset);
	
    return goals;
}

std::unique_ptr<Step_impl> GrabberStep::clone()const
{
    return std::unique_ptr<Step_impl>(new GrabberStep(*this));
}

bool GrabberStep::operator==(GrabberStep const& b)const
{
	assert(0) ;
	return true ;
}
