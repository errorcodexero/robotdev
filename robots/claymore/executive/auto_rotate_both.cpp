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
    rotate90pos,
    Step(Wait(2.0)),
    rotate90neg,
    endAuto
} ;

static Chain chain(steps, teleopex) ;
Executive rotate_both(chain) ;
