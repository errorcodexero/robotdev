#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Center
// Action: Score on right switch
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps =
{
    startAuto,
    calibrateLifter,
    calibrateGrabber,
    startLifterSwitch,
    Step(Drive_param("auto:c_switch_r:segment1", 105, true)),
    waitForLifter,
    eject,
    endAuto
} ;

static Chain chain(steps, teleopex) ;
Executive center_switch_right(chain) ;
