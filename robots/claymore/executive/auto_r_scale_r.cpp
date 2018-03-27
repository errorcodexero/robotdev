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
	Step(Rotate(-90, 15.0)),
    Step(Lifter_to_height(85.6)),
    eject,
	lifterToExch,
	Step(Rotate_finish(-90, -60)),
#ifdef NOTYET
	lifterToFloor,
	driveAndCollect,
	lifterToExch,
	driveBackFromCollect,
	rotateBack,
    Step(Lifter_to_height(85.6)),
	AUTO_POSTAMBLE,
    eject,
	lifterToExch,
#endif
} ;

static Chain chain(steps, teleopex) ;
Executive right_scale_right(chain) ;
