#include "autosteps.h"
#include "chain.h"
#include <vector>


using namespace std ;

//////////////////////////////////////////////////////////////////////////////////////
//
// Start: Right
// Action: Score on right scale
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps =
{
    AUTO_PREAMBLE("right_scale_right"),
	Step(Drive(290, false)),
	Step(Rotate(-90, 8.0)),
    Step(Lifter_to_height(85.0)),
    eject,
	lifterToFloor,
    Step(Rotate_finish(-90, -67, 3.0)),
	Step(Drive_and_collect(114.0)),
	Step(Close_collect_no_cube(1.0)),
	Step(Drive(-6.0)),
	Step(Close_collect_no_cube(0.5)),
	lifterToExch,
	Step(Drive(true, -30)),
	Step(Rotate_back(0.0, 10.0)),
	lifterToScale,
    eject,
	lifterToExch,
	AUTO_POSTAMBLE,
} ;

static Chain chain(steps, teleopex) ;
Executive right_scale_right(chain) ;
