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
    calibrateLifter,
    calibrateGrabber,
    startLifterExch,
    Step(Drive_param("auto:r_scale_r:segment1", 219.0, false)),
    rotate90pos,
    Step(Drive_param("auto:r_scale_r:segment2", 240.0, false)),		// TODO ?????
    rotate90neg,
    Step(Drive_param("auto:r_scale_r:segment2", 72.5, false)),
    rotate90neg,
    lifterToScale,
    eject,
    endAuto,
} ;

static Chain chain(steps, teleopex) ;
Executive left_scale_right(chain) ;
