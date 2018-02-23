#include "autosteps.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Right
// Action: Score on left scale
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> right_scale_left_steps =
{
    startAuto,
    calibrateLifter,
    calibrateGrabber,
    startLifterExch,
    Step(Drive_param("auto:r_scale_r:segment1", 219.0, false)),
    rotate90neg,
    Step(Drive_param("auto:r_scale_r:segment2", 240.0, false)),		// TODO ?????
    rotate90pos,
    Step(Drive_param("auto:r_scale_r:segment2", 72.5, false)),
    rotate90pos,
    lifterToScale,
    eject,
    endAuto,
} ;

static Chain right_scale_left_chain(right_scale_left_steps, teleopex) ;
Executive right_scale_left(right_scale_left_chain) ;
