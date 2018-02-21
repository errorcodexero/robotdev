#include "autosteps.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Right
// Action: Score on left scale
//
///////////////////////////////////////////////////////////////////////////////////////
vector<Step> right_scale_left_steps =
{
    startAuto,
    calibrateLifter,
    calibrateGrabber,
    startLifterExch,

    endAuto,
} ;

Chain right_scale_left_chain(right_scale_left_steps, teleopex) ;
Executive right_scale_left(right_scale_left_chain) ;
