#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Right
// Action: Score on right scale
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps =
{
    AUTO_PREAMBLE("right_scale_right"),
    startLifterExch,
    Step(Drive("auto:r_scale_r:segment1", 306, false)),
    rotate90neg,
    Step(Lifter_to_height(85.6)),
    eject,
	lifterToExch,
	Step(Rotate(-45.0)),
    //Step(Drive("auto:r_scale_r:segment2", 36, false)),
	AUTO_POSTAMBLE,
} ;

static Chain chain(steps, teleopex) ;
Executive right_scale_right(chain) ;
