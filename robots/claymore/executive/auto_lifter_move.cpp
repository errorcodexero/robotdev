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
    Step(Lifter_to_height(24.0,0.0)),
    Step(Wait(2.0)),
    Step(Lifter_to_height(43.5, 0.0)),
    Step(Wait(2.0)),
    Step(Lifter_to_height(24.0, 0.0)),
    endAuto,
};

static Chain chain(steps, teleopex) ;
Executive lifter_move(chain) ;