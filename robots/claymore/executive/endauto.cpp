#include "endauto.h"

extern double autoStartTime ;

//
// Wait: Wait for a specified amount of time
//

EndAuto::EndAuto() : Step("endauto")
{
}

Step::Status EndAuto::done(Next_mode_info info)
{
    messageLogger &logger = messageLogger::get() ;
    logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
    logger << "Automode program completed in " << info.in.now - autoStartTime << " seconds" ;
    logger.endMessage() ;
    
    return Step::Status::FINISHED_SUCCESS ;
}

Toplevel::Goal EndAuto::run(Run_info info)
{
    return run(info,{});
}

Toplevel::Goal EndAuto::run(Run_info info,Toplevel::Goal goals)
{
    return goals;
}
