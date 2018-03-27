#include "endauto.h"

extern double autoStartTime ;

//
// Wait: Wait for a specified amount of time
//

EndAuto::EndAuto(){
}

Step::Status EndAuto::done(Next_mode_info info){
    messageLogger &logger = messageLogger::get() ;
    logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
    logger << "Automode program completed in " << info.in.now - autoStartTime << " seconds" ;
    logger.endMessage() ;
    
    return Step::Status::FINISHED_SUCCESS ;
}

Toplevel::Goal EndAuto::run(Run_info info){
    return run(info,{});
}

Toplevel::Goal EndAuto::run(Run_info info,Toplevel::Goal goals){
    return goals;
}

std::unique_ptr<Step_impl> EndAuto::clone()const{
    return std::unique_ptr<Step_impl>(new EndAuto(*this));
}

bool EndAuto::operator==(EndAuto const& b)const{
    return true;
}
