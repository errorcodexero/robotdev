#include "autosteps.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Center
// Action: Score on right switch
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> rotate_neg90_steps =
{
    startAuto,
    rotate90pos,
    endAuto
} ;

static Chain rotate_neg90_chain(rotate_neg90_steps, teleopex) ;
Executive rotate_neg90(rotate_neg90_chain) ;
