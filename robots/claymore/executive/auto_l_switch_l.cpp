#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Left
// Action: Score on left switch
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps =
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

static Chain chain(steps, teleopex) ;
Executive left_switch_left(chain) ;
