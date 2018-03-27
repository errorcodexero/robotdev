#include "startauto.h"

double autoStartTime ;

//
// Start auto, mark the start of the auto program
//
StartAuto::StartAuto(const char *name_p) : Step("startauto")
{
	mProgramName = name_p ;
	mInited = false ;
}

Step::Status StartAuto::done(Next_mode_info info)
{
    autoStartTime = info.in.now ;
    return Step::Status::FINISHED_SUCCESS ;
}

Toplevel::Goal StartAuto::run(Run_info info){
    return run(info,{});
}

Toplevel::Goal StartAuto::run(Run_info info,Toplevel::Goal goals)
{
	if (!mInited)
	{
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Starting autonomous program " << mProgramName ;
		logger.endMessage() ;
		mInited = true ;
	}
    return goals;
}
