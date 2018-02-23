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
    Step(Drive(300.0, false)),
    waitForLifter,
    eject,
    endAuto
} ;

static Chain chain(steps, teleopex) ;
Executive drive_straight_300(chain) ;
