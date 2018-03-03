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
    AUTO_PREAMBLE
    Step(Drive("auto:cross_line:segment1",120.0, false)),
    endAuto
} ;

static Chain chain(steps, teleopex) ;
Executive cross_line(chain) ;
