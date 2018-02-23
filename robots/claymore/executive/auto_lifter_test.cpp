#include "autosteps.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Center
// Action: Score on left switch
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> lifter_test_steps
{
    startAuto,
    calibrateLifter,
    startLifterSwitch,
    Step(Drive(50.0, false)),
    waitForLifter,
    endAuto,
};

static Chain lifter_test_chain(lifter_test_steps, teleopex) ;
Executive lifter_test(lifter_test_chain) ;
