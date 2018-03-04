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
    AUTO_PREAMBLE,
    Step(Drive("auto:cross_line:segment1",144.0, false)),
	AUTO_POSTAMBLE,
} ;

static Chain chain(steps, teleopex) ;
Executive cross_line(chain) ;
