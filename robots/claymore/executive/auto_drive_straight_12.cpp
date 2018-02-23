#include "autosteps.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Center
// Action: Score on right switch
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> drive_straight_12_steps =
{
    startAuto,
    Step(Drive(12.0, false)),
    waitForLifter,
    eject,
    endAuto
} ;

static Chain drive_straight_12_chain(drive_straight_12_steps, teleopex) ;
Executive drive_straight_12(drive_straight_12_chain) ;
