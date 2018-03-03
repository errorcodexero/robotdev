#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Right
// Action: Score on right scale
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps =
{
    startAuto,
    calibrateLifter,
    calibrateGrabber,
    startLifterExch,
    Step(Drive("auto:r_scale_r:segment1", 306, false)),
    rotate90neg,
    Step(Lifter_to_height(89.6)),
    eject,
    startLifterExch,
    endAuto,
} ;

static Chain chain(steps, teleopex) ;
Executive right_scale_right(chain) ;
