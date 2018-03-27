#include "collect.h"

Collect::Collect(double time):time(time),init(false){}

Step::Status Collect::done(Next_mode_info info){
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

Toplevel::Goal Collect::run(Run_info info){
    return run(info,{});
}

Toplevel::Goal Collect::run(Run_info info,Toplevel::Goal goals){
    if(!init) {
		Grabber::grabber_controller.moveToAngle(GrabberController::Preset::OPEN, time);
		init = false;
    }
    goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::OPEN);
	return goals;
}

std::unique_ptr<Step_impl> Collect::clone()const{
    return std::unique_ptr<Step_impl>(new Collect(*this));
}

bool Collect::operator==(Collect const& b)const{
    return time == b.time && init == b.init;
}
