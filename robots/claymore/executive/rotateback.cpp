#include "rotateback.h"

//////////////////////////////////////////////////////////////////////////

Rotate_back::Rotate_back() : Step("rotate_back")
{
	mOffset = 0 ;
	mTolSpecified = false ;
}

Rotate_back::Rotate_back(double offset) : Step("rotate_back(offset)")
{
	mOffset = offset ;
	mTolSpecified = false ;
}

Rotate_back::Rotate_back(double offset, double tolval) : Step("rotate_back(offset, tolerance)")
{
	mOffset = offset ;
	mTolSpecified = true ;
	mTolerance = tolval ;
}

void Rotate_back::init(const Robot_inputs &in, Toplevel::Status_detail &status)
{
	double target = -Drivebase::drivebase_controller.getLastAngle() ;
	if (mTolSpecified)
		Drivebase::drivebase_controller.initAngle(status.drive.angle + target + mOffset, in.now, target > 0, mTolerance) ;
	else
		Drivebase::drivebase_controller.initAngle(status.drive.angle + target + mOffset, in.now, target > 0) ;
}

Toplevel::Goal Rotate_back::run(Run_info info)
{
    return run(info,{});
}

Toplevel::Goal Rotate_back::run(Run_info info,Toplevel::Goal goals)
{
    goals.drive = Drivebase::Goal::rotate();
    return goals;
}

Step::Status Rotate_back::done(Next_mode_info info)
{
    Step::Status ret =  ready(info.status.drive, Drivebase::Goal::rotate()) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;	
    if (ret == Step::Status::FINISHED_SUCCESS)
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Rotate_back step complete" ;
		logger.endMessage() ;
    }

    return ret ;
}
