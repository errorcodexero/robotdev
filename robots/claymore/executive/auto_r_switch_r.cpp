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
    startLifterExch,
    Step(Drive("auto:r_switch_r:segment1", 148, false)),
    rotate90neg,
	startLifterSwitch,
	Step(Wait(0.5)),
    Step(Drive("auto:r_switch_r:segment2", 18, true)),
    waitForLifter,
    eject,
    endAuto
} ;

static Chain chain(steps, teleopex) ;
Executive right_switch_right(chain) ;
