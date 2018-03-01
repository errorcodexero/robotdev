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
    Step(Drive_param("auto:r_scale_r:segment1", 306, false)),
    rotate90neg,
    Step(Lifter_to_height(89.6, 0.0)),
	
    #ifdef DRIVE_FORWARD
Step(Drive_param("auto:r_scale_r:segment2", 12, false)),
#endif
	
    eject,
	
#ifdef DRIVE_FORWARD
    Step(Drive_param("auto:r_scale_r:segment3", -12, false)),
#endif
	
    startLifterExch,
    endAuto,
} ;

static Chain chain(steps, teleopex) ;
Executive right_scale_right(chain) ;
