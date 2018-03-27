#include "rotatefinish.h"

extern double lastrotate ;

//////////////////////////////////////////////////////////////////////////
//
// Rotate: Rotate the robot by a specified angle
//
Rotate_finish::Rotate_finish(double prev, double a) : Step("rotate_finish(prev, angle)") 
{
	prev_angle = prev ;
	target_angle = a ;
	tolprovided = false ;
}

Rotate_finish::Rotate_finish(double prev, double a, double tol) : Step("rotate_finish(prev, angle, tolerance)") 
{
	prev_angle = prev ;
	target_angle = a ;
	tolerance = tol ;
	tolprovided = true ;
}

void Rotate_finish::init(const Robot_inputs &in, Toplevel::Status_detail &status)
{
	double target = lastrotate + target_angle + prev_angle ;
		
	if (tolprovided)
		Drivebase::drivebase_controller.initAngle(target, in.now, target_angle > 0, tolerance) ;
	else
		Drivebase::drivebase_controller.initAngle(target, in.now, target_angle > 0) ;
}

Toplevel::Goal Rotate_finish::run(Run_info info)
{
    return run(info,{});
}

Toplevel::Goal Rotate_finish::run(Run_info info,Toplevel::Goal goals)
{
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
