#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Left
// Action: Two cube score on left, scale then switch
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps =
{
    startAuto,
    calibrateLifter,
    calibrateGrabber,
    startLifterExch,
    Step(Drive_param("auto:r_scale_r:segment1", 306, false)),
    rotate90pos,
    lifterToScale,
    eject,
    Step(Rotate(75.0)),						// Would be great to get from parameter
    // open collector
    Step(Drive_param("auto:r_scale_r:segment2", 100, false)),		// No clue, need to figure out
    // close collector and collect
    lifterToSwitch,
    Step(Rotate(15.0)),						// Would be great to get from parameter
    Step(Drive_param("auto:r_scale_r:segment2", 12, true)),		// No clue, need to figure out
    endAuto,
} ;

static Chain chain(steps, teleopex) ;
Executive two_cube_left(chain) ;
