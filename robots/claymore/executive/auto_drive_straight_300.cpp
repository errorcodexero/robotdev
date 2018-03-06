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
    Step(StartAuto("drive_straight_300")),
    Step(Drive(300.0, false)),
    endAuto
} ;

static Chain chain(steps, teleopex) ;
Executive drive_straight_300(chain) ;
