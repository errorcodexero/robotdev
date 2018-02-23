#include "autosteps.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Center
// Action: Score on right switch
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> drive_straight_120_steps =
{
    startAuto,
    Step(Drive(120.0, false)),
    waitForLifter,
    eject,
    endAuto
} ;

static Chain drive_straight_120_chain(drive_straight_120_steps, teleopex) ;
Executive drive_straight_120(drive_straight_120_chain) ;
