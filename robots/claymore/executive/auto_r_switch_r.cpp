#include "autosteps.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Right
// Action: Score on right switch
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> right_switch_right_steps =
{
    startAuto,
    calibrateLifter,
    calibrateGrabber,
    startLifterSwitch,
    // TODO
    waitForLifter,
    eject,
    endAuto
} ;

static Chain right_switch_right_chain(right_switch_right_steps, teleopex) ;
Executive right_switch_right(right_switch_right_chain) ;
