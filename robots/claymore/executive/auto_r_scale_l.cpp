#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Right
// Action: Score on left scale
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps =
{
    AUTO_PREAMBLE("right_scale_left"),
    Step(Drive("auto:r_scale_l:segment1", 213, false)),
	Step(Rotate(-90.0, 3.0)),
    Step(Drive("auto:r_scale_l:segment2", 250, false)),
    Step(Rotate(90, 3.0)),
    Step(Drive("auto:r_scale_l:segment3", 80, false)),
	Step(Background_lifter_to_preset(83.0, 0.0)),
	Step(Rotate(90, 8.0)),
    Step(Lifter_to_height(83.0)),
	Step(Wait(0.15)),
    eject,
	lifterToFloor,
	AUTO_POSTAMBLE
} ;

static Chain chain(steps, teleopex) ;
Executive right_scale_left(chain) ;
