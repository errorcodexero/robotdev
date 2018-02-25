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
    Step(Drive_param("auto:r_switch_r:segment1", 148, false)),
    rotate90neg,
    Step(Drive_param("auto:r_switch_r:segment2", 18, true)),
    waitForLifter,
    eject,
    endAuto
} ;

static Chain chain(steps, teleopex) ;
Executive right_switch_right(chain) ;
