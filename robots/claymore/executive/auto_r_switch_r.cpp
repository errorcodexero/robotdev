#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Right
// Action: Score on right switch
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
Executive right_switch_right(chain) ;
