#include "closecollect.h"

Close_collect::Close_collect(double len)
{
	mInit = false ;
	mTime = len ;
}

Step::Status Close_collect::done(Next_mode_info info)
{
    Step::Status ret = Step::Status::UNFINISHED ;
	if (timeout_timer.done() || Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::HasCube)
		ret = Step::Status::FINISHED_SUCCESS ;
	
    if (ret == Step::Status::FINISHED_SUCCESS) 
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Close_collect step complete"  ;
		if (Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::HasCube)
			logger << " - has cube" ;
		else
			logger << " - timed out" ;

		logger << "\n" ;
		logger << "    Time " << info.in.now - mStart ;
		logger.endMessage() ;
    }
    return ret ;
	
}

Toplevel::Goal Close_collect::run(Run_info info){
    return run(info,{});
}

Toplevel::Goal Close_collect::run(Run_info info,Toplevel::Goal goals){
	if(!mInit) {
		timeout_timer.set(mTime) ;
		mInit = true ;
		mStart = info.in.now ;
	}

	timeout_timer.update(info.in.now, true);
    goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::CLOSED);
	goals.intake = Intake::Goal::in();
	if(Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::GraspCube)
		goals.grabber = Grabber::Goal::clamp();
	
    return goals;
}

std::unique_ptr<Step_impl> Close_collect::clone()const{
    return std::unique_ptr<Step_impl>(new Close_collect(*this));
}

bool Close_collect::operator==(Close_collect const& b)const{
    return true;
}
