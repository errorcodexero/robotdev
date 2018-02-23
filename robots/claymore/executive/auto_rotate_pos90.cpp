#include "autosteps.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Center
// Action: Score on right switch
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> rotate_pos90_steps =
{
    startAuto,
    rotate90pos,
    endAuto
} ;

static Chain rotate_pos90_chain(rotate_pos90_steps, teleopex) ;
Executive rotate_pos90(rotate_pos90_chain) ;
