#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Left
// Action: Score on left scale
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps =
{
    AUTO_PREAMBLE("left_scale_left"),
    startLifterExch,
    Step(Drive("auto:l_scale_l:segment1", 306, false)),
    rotate90pos,
	Step(Drive("auto:l_scale_l:segment2", -6, false)),
    Step(Lifter_to_height(85.6)),
    eject,
	lifterToExch,
	Step(Rotate(45.0)),
    //Step(Drive("auto:l_scale_l:segment3", 36, false)),
	AUTO_POSTAMBLE,
} ;

static Chain chain(steps, teleopex) ;
Executive left_scale_left(chain) ;
