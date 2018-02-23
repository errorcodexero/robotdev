#include "autonomous.h"
#include "teleop.h" 
#include "chain.h"
#include "step.h"
#include "message_logger.h"

using namespace std;


//
// Automodes defined in seperate files
//
extern Executive drive_straight_120 ;
extern Executive drive_straight_12 ;
extern Executive drive_straight_300 ;
extern Executive rotate_pos90 ;
extern Executive rotate_neg90 ;
extern Executive rotate_both ;
extern Executive lifter_test;
extern Executive lifter_move ;
extern Executive grabber_test ;
extern Executive calibrate_only ;
extern Executive right_scale_right ;
extern Executive right_scale_left ;
extern Executive left_scale_right ;
extern Executive left_scale_left ;
extern Executive center_switch_right ;
extern Executive center_switch_left ;
extern Executive left_switch_left ;
extern Executive right_switch_right ;


//
// Commonly used steps
//
Step startAuto = Step(StartAuto()) ;
Step endAuto = Step(EndAuto()) ;
Step calibrateLifter = Step(Calibrate_lifter()) ;
Step calibrateGrabber = Step(Calibrate_grabber()) ;
Step rotate90pos = Step(Rotate(90.0)) ;
Step rotate90neg = Step(Rotate(-90.0)) ;
Step startLifterFloor = Step(Background_lifter_to_preset(LifterController::Preset::FLOOR, 0.0)) ;
Step startLifterExch = Step(Background_lifter_to_preset(LifterController::Preset::EXCHANGE, 0.0)) ;
Step startLifterSwitch = Step(Background_lifter_to_preset(LifterController::Preset::SWITCH, 0.0)) ;
Step startLifterScale = Step(Background_lifter_to_preset(LifterController::Preset::SCALE, 0.0)) ;
Step lifterToFloor = Step(Lifter_to_preset(LifterController::Preset::FLOOR, 0.0)) ;
Step lifterToExch = Step(Lifter_to_preset(LifterController::Preset::EXCHANGE, 0.0)) ;
Step lifterToSwitch = Step(Lifter_to_preset(LifterController::Preset::SWITCH, 0.0)) ;
Step lifterToScale = Step(Lifter_to_preset(LifterController::Preset::SCALE, 0.0)) ;
Step waitForLifter = Step(Wait_for_lifter()) ;
Step eject = Step(Eject()) ;

//
// Ok we have done multiple things here trying to get something that works well
// for all cases and I think this is it. At the bottom of this function there is a
// big switch statement that sets the automode based on a variable named automode.
// This variable is set differently depending on whether or not the 'C' preprocessor
// variable AUTOMODE is set.  If AUTOMODE is set, the the variable automode is set to
// the value of AUTOMODE.  In this, with the AUTOMODE variable on the make command line
// you can force the auto mode to any mode you want.
//
// e.g. make CONFIG=debug AUTOMODE=102
//
// If AUTOMODE is not set (or if you are not building for configuration debug), then the
// value of automode is set based on the switch on the panel.  This switch only has 10
// positions so only the first 10 automodes in the switch statement are accessible.
//

Executive get_auto_mode(Next_mode_info info)
{
    Executive auto_program = calibrate_only ;
    messageLogger &logger = messageLogger::get() ;
    int automode = 0 ;
    
    if(!info.autonomous)
    {
	logger.startMessage(messageLogger::messageType::info) ;
	logger << "get_auto_mode - not in autonomous, returning Teleop mode" ;
	logger.endMessage() ;
	return Executive{Teleop()};
    }

#if defined(AUTOMODE) && defined(DEBUG)
    automode = AUTOMODE ;
    
    logger.startMessage(messageLogger::messageType::info) ;
    logger << "running in AUTOMODE test mode, test = " << AUTOMODE ;
    logger.endMessage() ;
#else
    //
    // If AUTOMODE was not defined, we revert to the default behavior which is
    // to run the with the panel
    //    

    logger.startMessage(messageLogger::messageType::info) ;
    logger << "get_auto_mode - competition mode, selecting auto mode based on switch" ;
    logger.endMessage() ;
	
    if(!info.panel.in_use) {
	//
	// No panel was detected.  May be a problem with the driver station, log this fact
	// and do nothing during autonomous
	//
	logger.startMessage(messageLogger::messageType::error) ;
	logger << "get_auto_mode - no panel detected, defaulting to null auto program" ;
	logger.endMessage() ;
	
	return calibrate_only ;
    }

    logger.startMessage(messageLogger::messageType::error) ;
    logger << "get_auto_mode - panel value is " << info.panel.auto_select ;
    logger.endMessage() ;

    automode = info.panel.auto_select ;
#endif
    
    switch(automode)
    {
    case 0:
	auto_program = calibrate_only ;
	break; 
    case 1: 
	break;
    case 2:
	auto_program = info.in.ds_info.near_switch_left ? center_switch_left : center_switch_right ;
	break;
    case 3:
	auto_program = info.in.ds_info.scale_left ? right_scale_left : right_scale_right ;
	break;
    case 4:
	auto_program = info.in.ds_info.scale_left ? left_scale_left : left_scale_right ;
	break ;
    case 5: 
    case 6:
    case 7: 
    case 8: 
    case 9: 
	break ;
	
    case 100:
	//
	// calibrate only
	//
	auto_program = calibrate_only ;
	break ;

    case 101:
	//
	// drive straight for 120 inches (10 feet)
	//
	auto_program = drive_straight_120 ;
	break ;
	
    case 102:
	//
	// drive straight for 12 inches (1 foot)
	//
	auto_program = drive_straight_12 ;
	break ;
	
    case 103:
	//
	// drive straight for 12 inches (1 foot)
	//
	auto_program = drive_straight_300 ;
	break ;

    case 104:
	//
	// Rotate 90 degrees
	//
	auto_program = rotate_pos90 ;
	break ;
	
    case 105:
	//
	// Rotate 90 degrees
	//
	auto_program = rotate_neg90 ;
	break ;

    case 106:
	//
	// Rotate 90 degrees
	//
	auto_program = rotate_both ;
	break ;

    case 107:
	//
	// Start lifter in brackground, drive 60 inches, wait for lifter
	//
	auto_program = lifter_test ;
	break ;
	
    case 108:
	//
	// Calibrate the lifter, move to 24 inches, 43.5 inches, 24 inches
	//
	auto_program = lifter_move ;
	break ;

    case 109:
	//
	// Test the grabber
	//
	auto_program = grabber_test ;
	break;

    case 120:
	//
	// Competition auto program, start in center, score on right side of switch
	//
	auto_program = center_switch_right ;
	break ;

    case 121:
	//
	// Competition auto program, start in center, score on left side of switch
	//
	auto_program = center_switch_left ;
	break ;

    case 122:
	//
	// Competition auto program, start on right side, score on right scale
	//
	auto_program = right_scale_right ;
	break ;
	
    case 123:
	//
	// Competition auto program, start on left side, score scale on left sdie
	//
	auto_program = left_scale_left ;
	break ;

    case 124:
	//
	// Compeition auto program, start on right side, score left scale
	//
	auto_program = right_scale_left ;
	break ;
	
    case 125:
	//
	// Compeition auto program, start on left side, score right scale
	//
	auto_program = left_scale_right ;
	break ;

    default:
	auto_program = calibrate_only ;
	break ;
    }
	
    return auto_program ;
}

Executive Autonomous::next_mode(Next_mode_info info)
{
    static const Time DELAY = 0.0;//seconds, TODO: base it off of the dial on the OI? 
    if(!info.autonomous) return Executive{Teleop()};
    if(info.since_switch > DELAY) return get_auto_mode(info);
    return Executive{Autonomous()};
}

Toplevel::Goal Autonomous::run(Run_info)
{	
    return {} ;
}

bool Autonomous::operator<(Autonomous const&)const
{
	return 0;
}

bool Autonomous::operator==(Autonomous const&)const
{
	return 1;
}

void Autonomous::display(ostream& o)const
{
    o<<"Autonomous()";
}


#ifdef AUTONOMOUS_TEST
#include "test.h"
int main()
{
    Autonomous a;
    test_executive(a);
}
#endif 
