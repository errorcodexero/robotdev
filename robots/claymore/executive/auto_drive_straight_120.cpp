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
    Step(StartAuto("drive_straight_120")),
    Step(Drive(120.0, false)),
    endAuto
} ;

static Chain chain(steps, teleopex) ;
Executive drive_straight_120(chain) ;
