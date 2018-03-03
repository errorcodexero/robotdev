#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Left
// Action: Score on left scale
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps =
{
    startAuto,
    calibrateLifter,
    calibrateGrabber,
    startLifterExch,
    Step(Drive("auto:l_scale_l:segment1", 306, false)),
    rotate90pos,
    Step(Lifter_to_height(89.3)),
    eject,
    endAuto,
} ;

static Chain chain(steps, teleopex) ;
Executive left_scale_left(chain) ;
