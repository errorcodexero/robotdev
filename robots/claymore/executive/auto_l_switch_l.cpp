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
    AUTO_PREAMBLE
    startLifterSwitch,
    Step(Drive("auto:l_switch_l:segment1", 148, false)),
    rotate90pos,
    Step(Drive("auto:l_switch_l:segment1", 18, true)),
    waitForLifter,
    eject,
    endAuto
} ;

static Chain chain(steps, teleopex) ;
Executive left_switch_left(chain) ;
