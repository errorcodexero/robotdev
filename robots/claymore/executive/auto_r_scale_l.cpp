#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Right
// Action: Score on left scale
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps =
{
    AUTO_PREAMBLE("right_scale_left"),
    startLifterExch,
    Step(Drive("auto:r_scale_r:segment1", 219.0, false)),
    rotate90neg,
    Step(Drive("auto:r_scale_r:segment2", 240.0, false)),		// TODO ?????
    rotate90pos,
    Step(Drive("auto:r_scale_r:segment2", 72.5, false)),
    rotate90pos,
    lifterToScale,
    eject,
	AUTO_POSTAMBLE,
} ;

static Chain chain(steps, teleopex) ;
Executive right_scale_left(chain) ;
