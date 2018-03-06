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
    AUTO_PREAMBLE("center_switch_right"),
    startLifterSwitch,
    Step(Drive("auto:c_switch_r:segment1", 105, true)),
    waitForLifter,
    eject,
	AUTO_POSTAMBLE,
} ;

static Chain chain(steps, teleopex) ;
Executive center_switch_right(chain) ;
