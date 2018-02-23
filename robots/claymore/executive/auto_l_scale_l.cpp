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
    Step(Drive_param("auto:l_scale_l:segment1", 306, false)),
    rotate90pos,
    lifterToScale,
    eject,
    endAuto,
} ;

static Chain chain(steps, teleopex) ;
Executive left_scale_left(chain) ;
