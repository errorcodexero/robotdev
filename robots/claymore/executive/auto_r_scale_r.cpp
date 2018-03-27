#include "autosteps.h"
#include "chain.h"
#include "closecollect.h"
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
	
#ifdef CONSERVATIVE
	Step(Drive(290, false)),
	Step(Rotate(-90, 8.0)),
    Step(LifterStep(85.0)),
    eject,
	lifterToFloor,
    Step(Rotate_finish(-90, -67, 3.0)),
	Step(Drive_and_collect(114.0)),
	Step(Close_collect(1.0)),
	Step(Drive(-6.0)),
	Step(Close_collect(0.5)),
	lifterToExch,
	Step(Drive(true, -30)),
	Step(Rotate_back(0.0, 10.0)),
	lifterToScale,
    eject,
	lifterToExch,
#else
	Step(Drive(244.0, 100.0, -140.0, false)),
    Step(LifterStep(85.0)),
	Step(Eject(0.7)),
	lifterToFloor,
	Step(Rotate(-106)),
	Step(Drive_and_collect(57.0)),
	Step(Close_collect(1.0)),
	Step(Drive(-6.0)),
	Step(Close_collect(0.5)),
	Step(LifterStep(85.0, true)),
	Step(Drive(true, -6.0)),
	Step(Rotate_back(-30.0, 10.0)),
    Step(LifterStep(85.0)),
	eject,
	lifterToExch,
#endif
	
	AUTO_POSTAMBLE,
} ;

static Chain chain(steps, teleopex) ;
Executive right_scale_right(chain) ;
