#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Center
// Action: Score on left switch
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps
{
	AUTO_PREAMBLE("center_switch_left"),
    startLifterExch,
    Step(Drive("auto:c_switch_l:segment1", 52.0, false)),
    rotate90neg,
	Step(Drive("auto:c_switch_l:segment2", 100.0, false)),
    rotate90pos,
    startLifterSwitch,
    Step(Drive("auto:c_switch_l:segment3", 56.0, true)),
    waitForLifter,
    eject,
    Step(Drive("auto:c_switch_r:segment1", -24, true)),
	lifterToExch,
	Step(Rotate(45.0)),
	lifterToFloor,
	AUTO_POSTAMBLE,
};

static Chain chain(steps, teleopex) ;
Executive center_switch_left(chain) ;
