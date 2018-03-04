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
	AUTO_PREAMBLE,
	Step(Lifter_to_preset(LifterController::Preset::EXCHANGE, 0.0)),
	Step(Lifter_to_preset(LifterController::Preset::FLOOR, 0.0)),
	calibrateGrabber,
    startLifterExch,
    Step(Drive("auto:l_switch_l:segment1", 148, false)),
    rotate90pos,
    startLifterSwitch,
    Step(Wait(0.5)),
    Step(Drive("auto:l_switch_l:segment2", 18, true)),
    waitForLifter,
    eject,
	AUTO_POSTAMBLE,
} ;

static Chain chain(steps, teleopex) ;
Executive left_switch_left(chain) ;
