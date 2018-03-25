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
	lifterToSwitch,
    eject,
    Step(Drive("auto:c_switch_r:segment2", -24, true)),
	lifterToExch,
	Step(Rotate(-90.0)),
	lifterToFloor,
	driveAndCollect,
	lifterToExch,
	Step(Drive(true, -10)),
	Step(Rotate(90.0)),
	startLifterSwitch,
	Step(Drive("auto:c_switch_r:segment3", 24, true)),
	lifterToSwitch,
	eject,
	Step(Drive("auto_c_switch_r:segment4", -24, true)),
	AUTO_POSTAMBLE,
} ;

static Chain chain(steps, teleopex) ;
Executive center_switch_right(chain) ;
