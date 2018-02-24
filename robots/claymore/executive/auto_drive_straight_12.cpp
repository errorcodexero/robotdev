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
    Step(Drive(12.0, false)),
    endAuto
} ;

static Chain chain(steps, teleopex) ;
Executive drive_straight_12(chain) ;
