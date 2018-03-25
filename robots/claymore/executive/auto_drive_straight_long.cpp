#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: N/A
// Action: Drive straight far enough to activate high gear
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps =
{
    AUTO_PREAMBLE("drive_straight_long"),
    Step(Drive(290.0, false)),
    AUTO_POSTAMBLE
} ;

static Chain chain(steps, teleopex) ;
Executive drive_straight_long(chain) ;
