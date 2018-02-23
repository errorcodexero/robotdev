#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Center
// Action: Score on left switch
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps
{
    startAuto,
    calibrateLifter,
    calibrateGrabber,
    endAuto,
};

static Chain chain(steps, teleopex) ;
Executive grabber_test(chain) ;
