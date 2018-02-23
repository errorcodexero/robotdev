#include "autosteps.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Left
// Action: Score on left switch
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> left_switch_left_steps =
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

static Chain left_switch_left_chain(left_switch_left_steps, teleopex) ;
Executive left_switch_right(left_switch_left_chain) ;
