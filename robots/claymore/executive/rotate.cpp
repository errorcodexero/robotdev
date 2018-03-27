#include "rotate.h"

//
// Rotate: Rotate the robot by a specified angle
//
Rotate::Rotate(double a)
{
	target_angle = a ;
	init = false ;
	tolprovided = false ;
}

Rotate::Rotate(double a, double tol)
{
	target_angle = a ;
	init = false ;
	tolprovided = true ;
	tolerance = tol ;
}

Toplevel::Goal Rotate::run(Run_info info)
{
    return run(info,{});
}

double lastrotate ;
Toplevel::Goal Rotate::run(Run_info info,Toplevel::Goal goals)
{
    if(!init) {
		if (tolprovided)
		{
			Drivebase::drivebase_controller.initAngle(info.status.drive.angle + target_angle, info.in.now, target_angle > 0, tolerance) ;
		}
		else
		{
			Drivebase::drivebase_controller.initAngle(info.status.drive.angle + target_angle, info.in.now, target_angle > 0) ;
		}
		lastrotate = info.status.drive.angle ;
		init = true;
    }

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

std::unique_ptr<Step_impl> Rotate::clone()const
{
    return std::unique_ptr<Step_impl>(new Rotate(*this));
}

bool Rotate::operator==(Rotate const& b)const
{
    return target_angle == b.target_angle && init == b.init;
}

