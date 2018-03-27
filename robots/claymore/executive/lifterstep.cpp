#include "lifterstep.h"


LifterStep::LifterStep(LifterController::Preset pre, bool back) : Step("lifterstep(preset)")
{
	preset = pre ;
	heightgiven = false ;
	background = back ;
}

LifterStep::LifterStep(double h, bool back) : Step("lifterstep(height)")
{
	heightgiven = true ;
	height = h ;
	background = back ;
}

LifterStep::LifterStep(const char *name_p, double def, bool back) : Step("lifterstep(param)")
{
	height = def ;
	param = name_p ;
	background = back ;
	heightgiven = false ;
}

void LifterStep::init(const Robot_inputs &in, Toplevel::Status_detail &status)
{
	if (heightgiven)
	{
		Lifter::lifter_controller.moveToHeight(height, in.now, background);
	}
	else if (param.length() > 0)
	{
		paramsInput *params_p = paramsInput::get() ;
		height = params_p->getValue(param, height) ;
		Lifter::lifter_controller.moveToHeight(height, in.now, background);
	}
	else
		Lifter::lifter_controller.moveToHeight(preset, in.now, background);
}

Step::Status LifterStep::done(Next_mode_info info)
{
	Step::Status ret = Step::Status::UNFINISHED ;

	if (background)
	{
		//
		// In all cases, background operations are done immediately
		//
		ret = Step::Status::FINISHED_SUCCESS ;
	}
	else if (heightgiven && ready(status(info.status.lifter), Lifter::Goal::go_to_height(height)))
	{
		//
		// A height was supplied, check to see if it has been reached
		//
		ret = Step::Status::FINISHED_SUCCESS ;
	}
	else if (param.length() > 0 && ready(status(info.status.lifter), Lifter::Goal::go_to_height(height)))
	{
		//
		// A parameter from the params file was given, we moved the height we looked up to the height member
		// during initialization.  See if we are there
		//
		ret = Step::Status::FINISHED_SUCCESS ;
	}
	else if (!heightgiven && param.length() == 0 && ready(status(info.status.lifter), Lifter::Goal::go_to_preset(preset)))
	{
		//
		// A preset was supplied, see if we are at the preset
		//
		ret = Step::Status::FINISHED_SUCCESS ;
	}

	if (ret == Step::Status::FINISHED_SUCCESS)
	{
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "LifterStep step complete" ;
		logger.endMessage() ;
	}

	return ret ;
}

Toplevel::Goal LifterStep::run(Run_info info)
{
    return run(info,{});
}

Toplevel::Goal LifterStep::run(Run_info info,Toplevel::Goal goals)
{
    return goals;
}
