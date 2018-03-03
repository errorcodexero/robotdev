#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Left
// Action: Score on right scale
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps =
{
    startAuto,
    AUTO_PREAMBLE
    startLifterExch,
    Step(Drive("auto:l_scale_r:segment1", 223, false)),
    rotate90pos,
    Step(Drive("auto:l_scale_r:segment2", 199.75, false)),		// TODO ?????
    rotate90neg,
    Step(Drive("auto:l_scale_r:segment3", 30.0, false)),
    Step(Lifter_to_height(89.3)),
    eject,
    endAuto,
    Step(Drive("auto:l_scale_r:segment4", -30.0, false)),
    lifterToExch,
} ;

static Chain chain(steps, teleopex) ;
Executive left_scale_right(chain) ;
