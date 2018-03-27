#include "rotateback.h"

//////////////////////////////////////////////////////////////////////////

Rotate_back::Rotate_back()
{
	init = false ;
	mOffset = 0 ;
	mTolSpecified = false ;
}

Rotate_back::Rotate_back(double offset)
{
	init = false ;
	mOffset = offset ;
	mTolSpecified = false ;
}

Rotate_back::Rotate_back(double offset, double tolval)
{
	init = false ;
	mOffset = offset ;
	mTolSpecified = true ;
	mTolerance = tolval ;
}

Toplevel::Goal Rotate_back::run(Run_info info)
{
    return run(info,{});
}

Toplevel::Goal Rotate_back::run(Run_info info,Toplevel::Goal goals)
{
    if(!init) {
		double target = -Drivebase::drivebase_controller.getLastAngle() ;
		if (mTolSpecified)
			Drivebase::drivebase_controller.initAngle(info.status.drive.angle + target + mOffset, info.in.now, target > 0, mTolerance) ;
		else
			Drivebase::drivebase_controller.initAngle(info.status.drive.angle + target + mOffset, info.in.now, target > 0) ;
		init = true;
    }

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

std::unique_ptr<Step_impl> Rotate_back::clone()const
{
    return std::unique_ptr<Step_impl>(new Rotate_back(*this));
}

bool Rotate_back::operator==(Rotate_back const& b)const
{
    return true ;
}

