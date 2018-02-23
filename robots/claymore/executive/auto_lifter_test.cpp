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
    startLifterSwitch,
    Step(Drive(50.0, false)),
    waitForLifter,
    endAuto,
};

static Chain chain(steps, teleopex) ;
Executive lifter_test(chain) ;
