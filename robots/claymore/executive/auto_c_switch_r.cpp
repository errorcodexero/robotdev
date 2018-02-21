#include "autosteps.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Center
// Action: Score on right switch
//
///////////////////////////////////////////////////////////////////////////////////////
vector<Step> center_switch_right_steps =
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

Chain center_switch_right_chain(center_switch_right_steps, teleopex) ;
Executive center_switch_right(center_switch_right_chain) ;
