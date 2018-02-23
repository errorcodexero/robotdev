#include "autosteps.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Right
// Action: Score on right scale
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> right_scale_right_steps =
{
    startAuto,
    calibrateLifter,
    calibrateGrabber,
    startLifterExch,
    Step(Drive_param("auto:r_scale_r:segment1", 306, false)),
    rotate90neg,
    lifterToScale,
    eject,
    endAuto,
} ;

static Chain right_scale_right_chain(right_scale_right_steps, teleopex) ;
Executive right_scale_right(right_scale_right_chain) ;
