#include "calibrategrabber.h"


Calibrate_grabber::Calibrate_grabber()
{
    mInited = false ;
}

Step::Status Calibrate_grabber::done(Next_mode_info info)
{
    Step::Status ret =  ready(status(info.status.grabber), Grabber::Goal::calibrate()) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
    if (ret == Step::Status::FINISHED_SUCCESS)
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Calibrate grabber step done" ;
		logger.endMessage() ;
    }
    return ret ;
}

Toplevel::Goal Calibrate_grabber::run(Run_info info)
{
    return run(info,{});
}

Toplevel::Goal Calibrate_grabber::run(Run_info info,Toplevel::Goal goals)
{
    if (!mInited)
    {
		Grabber::grabber_controller.calibrate() ;
		mInited = true ;
    }
    goals.grabber = Grabber::Goal::calibrate();
    return goals;
}

std::unique_ptr<Step_impl> Calibrate_grabber::clone()const
{
    return std::unique_ptr<Step_impl>(new Calibrate_grabber(*this));
}

bool Calibrate_grabber::operator==(Calibrate_grabber const& b)const
{
    return true;
}
