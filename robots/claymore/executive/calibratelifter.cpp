#include "calibratelifter.h"

Calibrate_lifter::Calibrate_lifter()
{
	mInited = false ;
}

Step::Status Calibrate_lifter::done(Next_mode_info info)
{
    Step::Status ret = ready(status(info.status.lifter), Lifter::Goal::calibrate()) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
    if (ret == Step::Status::FINISHED_SUCCESS)
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Calibrate Lifter Step done" ;
		logger.endMessage() ;
    }
    return ret ;
}

Toplevel::Goal Calibrate_lifter::run(Run_info info)
{
    return run(info,{});
}

Toplevel::Goal Calibrate_lifter::run(Run_info info,Toplevel::Goal goals)
{
    goals.lifter = Lifter::Goal::calibrate();
    return goals;
}

std::unique_ptr<Step_impl> Calibrate_lifter::clone()const
{
    return std::unique_ptr<Step_impl>(new Calibrate_lifter(*this));
}

bool Calibrate_lifter::operator==(Calibrate_lifter const& b)const
{
    return true;
}

