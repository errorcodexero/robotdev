#include "collect.h"

Collect::Collect(double t) : Step("collect")
{
	time = t ;
}

Step::Status Collect::done(Next_mode_info info)
{
    Step::Status ret = info.status.grabber.has_cube ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
    if (ret == Step::Status::FINISHED_SUCCESS)
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Collect complete" ;
		logger.endMessage() ;
    }
    return ret ;
	
}

Toplevel::Goal Collect::run(Run_info info)
{
    return run(info,{});
}

void Collect::init(const Robot_inputs &in, Toplevel::Status_detail &status)
{
	Grabber::grabber_controller.moveToAngle(GrabberController::Preset::OPEN, time);
}

Toplevel::Goal Collect::run(Run_info info,Toplevel::Goal goals)
{
    goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::OPEN);
	return goals;
}
