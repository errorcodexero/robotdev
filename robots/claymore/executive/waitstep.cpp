#include "waitstep.h"

//
// Wait: Wait for a specified amount of time
//

Wait::Wait(Time wait_time){
    wait_timer.set(wait_time);
}

Step::Status Wait::done(Next_mode_info){
    Step::Status ret =  wait_timer.done() ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
    if (ret == Step::Status::FINISHED_SUCCESS)
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Wait step complete" ;
		logger.endMessage() ;
    }
    return ret ;
	
}

Toplevel::Goal Wait::run(Run_info info){
    return run(info,{});
}

Toplevel::Goal Wait::run(Run_info info,Toplevel::Goal goals){
    wait_timer.update(info.in.now,info.in.robot_mode.enabled);
    return goals;
}

std::unique_ptr<Step_impl> Wait::clone()const{
    return std::unique_ptr<Step_impl>(new Wait(*this));
}

bool Wait::operator==(Wait const& b)const{
    return wait_timer == b.wait_timer;
}

