#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Center
// Action: Score on right switch
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps =
{
    startAuto,
    calibrateLifter,
    calibrateGrabber,
    startLifterExch,
    waitForLifter,
    rotate90neg,
	eject,
    endAuto
} ;

static Chain chain(steps, teleopex) ;
Executive rotate_neg90(chain) ;
