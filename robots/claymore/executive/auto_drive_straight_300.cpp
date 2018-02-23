#include "autosteps.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Center
// Action: Score on right switch
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> drive_straight_300_steps =
{
    startAuto,
    Step(Drive(300.0, false)),
    waitForLifter,
    eject,
    endAuto
} ;

static Chain drive_straight_300_chain(drive_straight_300_steps, teleopex) ;
Executive drive_straight_300(drive_straight_300_chain) ;
