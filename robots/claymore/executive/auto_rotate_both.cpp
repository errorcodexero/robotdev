#include "autosteps.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Center
// Action: Score on right switch
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> rotate_both_steps =
{
    startAuto,
    rotate90pos,
    Step(Wait(2.0)),
    rotate90neg,
    endAuto
} ;

static Chain rotate_both_chain(rotate_both_steps, teleopex) ;
Executive rotate_both(rotate_both_chain) ;
