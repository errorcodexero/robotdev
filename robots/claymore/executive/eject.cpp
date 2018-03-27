#include "eject.h"

Eject::Eject() : Step("eject")
{
	mState = EjectState::Start ;
	mPowerApplied = false ;
}

Eject::Eject(double power) : Step("eject(power)")
{
	mPower = power ;
	mPowerApplied = true ;
}

Step::Status Eject::done(Next_mode_info info)
{
    Step::Status ret = Step::Status::UNFINISHED ;
	if (mState == EjectState::Done)
		ret = Step::Status::FINISHED_SUCCESS ;
		
    if (ret == Step::Status::FINISHED_SUCCESS)
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Eject step complete, elapsed " << info.in.now - mStart ;
		logger.endMessage() ;
    }
    return ret ;
}

Toplevel::Goal Eject::run(Run_info info){
    return run(info,{});
}

Toplevel::Goal Eject::run(Run_info info,Toplevel::Goal goals)
{
	switch(mState)
	{
	case EjectState::Start:
		mStart = info.in.now ;
		if (info.status.grabber.has_cube)
		{
			//
			// The cube sensor is on, wait for it to go off
			// We also set a timer just in case the sensor never
			// goes off
			//
			mState = EjectState::WaitingCubeSensorOff ;
			eject_timer.set(1.0) ;
		}
		else
		{
			//
			// The cube sensor is off, it may not be working as
			// we are trying to eject.  Just assume a fixed time
			// for the eject operation
			//
			mState = EjectState::WaitingOnTime ;
			eject_timer.set(1.0) ;
		}
		break ;

	case EjectState::WaitingCubeSensorOff:
		//
		// We get here if the cube sensor was on when we started
		// an eject operation.  There are two ways to leave this state.
		// First, if the cube sensor goes off, or if the fail safe time
		// we started expires
		//
		if (!info.status.grabber.has_cube)
		{
			//
			// The cube has disappeared.  Start a timer to leave the intake
			// in eject mode for a little longer to be sure the cube is fully
			// ejected
			//
			eject_timer.set(0.05) ;
			mState = EjectState::WaitingOnTime ;
		}
		else if (eject_timer.done())
		{
			//
			// We were waiting for the cube sensor to turn off but it has
			// not after our fail safe timeout.  Assume the sensor is broken
			// and stuck on and stop the eject operation.
			//
			mState = EjectState::Done ;
		}
		break ;

	case EjectState::WaitingOnTime:
		//
		// The timer has expired while we were waiting on the timer.  We are done.
		//
		if (eject_timer.done())
			mState = EjectState::Done ;
		break ;

	case EjectState::Done:
		Grabber::grabber_controller.setIdle() ;
		break ;
	}		

    //
    // Update the timer.  When it expires we consider this step done no matter
    // what else is happening
    //
    eject_timer.update(info.in.now, info.in.robot_mode.enabled);

    //
    // Tell the grabber/intake to eject the cube
    //
    goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::CLOSED);
	if (mPowerApplied)
		goals.intake = Intake::Goal::out(mPower);
	else
		goals.intake = Intake::Goal::out();
    
    return goals;
}
