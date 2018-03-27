#include "grabberstep.h"

GrabberStep::GrabberStep(GrabberController::Preset target_preset) : Step("grabber(preset)")
{
	preset = target_preset ;
	anglegiven = false ;
}

GrabberStep::GrabberStep(double a) : Step("grabber(angle)")
{
	angle = a ;
	anglegiven = true ;
}

GrabberStep::GrabberStep(const char *param_p, double a) : Step("grabber(param)")
{
	anglegiven = false ;
	param = param_p ;
	angle = a ;
}

void GrabberStep::init(const Robot_inputs &in, Toplevel::Status_detail &status)
{
	if (anglegiven)
	{
		Grabber::grabber_controller.moveToAngle(angle, in.now) ;
	}
	else if (param.length() > 0)
	{
		paramsInput *params_p = paramsInput::get() ;
		angle = params_p->getValue(param, angle) ;
		Grabber::grabber_controller.moveToAngle(angle, in.now) ;
	}
	else
	{
		Grabber::grabber_controller.moveToAngle(preset, in.now) ;
	}
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
	if (anglegiven || param.length() > 0)
		goals.grabber = Grabber::Goal::go_to_angle(angle) ;
	else
		goals.grabber = Grabber::Goal::go_to_preset(preset);
	
    return goals;
}
