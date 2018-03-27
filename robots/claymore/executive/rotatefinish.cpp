#include "rotatefinish.h"

extern double lastrotate ;

//////////////////////////////////////////////////////////////////////////
//
// Rotate: Rotate the robot by a specified angle
//
Rotate_finish::Rotate_finish(double prev, double a)
{
	prev_angle = prev ;
	target_angle = a ;
	init = false ;
	tolprovided = false ;
}

Rotate_finish::Rotate_finish(double prev, double a, double tol)
{
	prev_angle = prev ;
	target_angle = a ;
	tolerance = tol ;
	init = false ;
	tolprovided = true ;
}

Toplevel::Goal Rotate_finish::run(Run_info info)
{
    return run(info,{});
}

Toplevel::Goal Rotate_finish::run(Run_info info,Toplevel::Goal goals)
{
    if(!init)
	{
		double target = lastrotate + target_angle + prev_angle ;
		
		if (tolprovided)
			Drivebase::drivebase_controller.initAngle(target, info.in.now, target_angle > 0, tolerance) ;
		else
			Drivebase::drivebase_controller.initAngle(target, info.in.now, target_angle > 0) ;
		init = true;
    }

    goals.drive = Drivebase::Goal::rotate();
    return goals;
}

Step::Status Rotate_finish::done(Next_mode_info info)
{
    Step::Status ret =  ready(info.status.drive, Drivebase::Goal::rotate()) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;	
    if (ret == Step::Status::FINISHED_SUCCESS)
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Rotate_finish step complete" ;
		logger.endMessage() ;
    }

    return ret ;
}

std::unique_ptr<Step_impl> Rotate_finish::clone()const
{
    return std::unique_ptr<Step_impl>(new Rotate_finish(*this));
}

bool Rotate_finish::operator==(Rotate_finish const& b)const
{
    return target_angle == b.target_angle && init == b.init;
}


