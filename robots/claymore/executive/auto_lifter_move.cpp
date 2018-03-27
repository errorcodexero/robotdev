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
    Step(StartAuto("lifter_move")),
    calibrateLifter,
    Step(LifterStep(24.0)),
    Step(Wait(2.0)),
    Step(LifterStep(43.5)),
    Step(Wait(2.0)),
    Step(LifterStep(24.0)),
    endAuto,
};

static Chain chain(steps, teleopex) ;
Executive lifter_move(chain) ;
