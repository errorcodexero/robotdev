#include "autosteps.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Right
// Action: Score on right scale
//
///////////////////////////////////////////////////////////////////////////////////////
vector<Step> right_scale_right_steps =
{
    startAuto,
    calibrateLifter,
    calibrateGrabber,
    startLifterExch,
    Step(Drive_param("auto:r_scale_r:segment1", 296, false)),
    rotate90neg,
    lifterToScale,
    eject,
    endAuto,
} ;

Chain right_scale_right_chain(right_scale_right_steps, teleopex) ;
Executive right_scale_right(right_scale_right_chain) ;
