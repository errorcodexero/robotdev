#include "rotate.h"

double lastrotate ;

//
// Rotate: Rotate the robot by a specified angle
//
Rotate::Rotate(double a) : Step("rotate(angle)")
{
	target_angle = a ;
	tolprovided = false ;
}

Rotate::Rotate(double a, double tol) : Step("rotate(angle, tolerance)")
{
	target_angle = a ;
	tolprovided = true ;
	tolerance = tol ;
}

void Rotate::init(const Robot_inputs &in, Toplevel::Status_detail &status)
{
	if (tolprovided)
	{
		Drivebase::drivebase_controller.initAngle(status.drive.angle + target_angle, in.now, target_angle > 0, tolerance) ;
	}
	else
	{
		Drivebase::drivebase_controller.initAngle(status.drive.angle + target_angle, in.now, target_angle > 0) ;
	}
	
	lastrotate = status.drive.angle ;
}

Toplevel::Goal Rotate::run(Run_info info)
{
    return run(info,{});
}

Toplevel::Goal Rotate::run(Run_info info,Toplevel::Goal goals)
{
    goals.drive = Drivebase::Goal::rotate();
    return goals;
}

Step::Status Rotate::done(Next_mode_info info)
{
    Step::Status ret =  ready(info.status.drive, Drivebase::Goal::rotate()) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;	
    if (ret == Step::Status::FINISHED_SUCCESS)
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Rotate step complete" ;
		logger.endMessage() ;
    }

    return ret ;
}
