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
    Step(Lifter_to_height(89.3, 0.0)),
    eject,
    lifterToExch,
    endAuto,
} ;

static Chain chain(steps, teleopex) ;
Executive lift_eject(chain) ;
