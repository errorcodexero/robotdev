#include "autosteps.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Left
// Action: Score on left scale
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> left_scale_left_steps =
{
    startAuto,
    calibrateLifter,
    calibrateGrabber,
    startLifterExch,
    Step(Drive_param("auto:l_scale_l:segment1", 306, false)),
    rotate90pos,
    lifterToScale,
    eject,
    endAuto,
} ;

static Chain left_scale_left_chain(left_scale_left_steps, teleopex) ;
Executive left_scale_left(left_scale_left_chain) ;
