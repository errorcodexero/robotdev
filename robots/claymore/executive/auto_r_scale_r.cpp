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
static vector<Step> steps_two =
{
    AUTO_PREAMBLE("right_scale_right"),
	Step(Background_lifter_to_preset(85.0, 0.0)),
	Step(Drive(253.0, 100.0, -30.0, false)),
    Step(Lifter_to_height(85.0)),
	Step(Eject(0.7)),
	lifterToFloor,
	Step(Rotate(-120)),
	Step(Drive_and_collect(56.0)),
	Step(Close_collect_no_cube(1.0)),
	Step(Drive(-6.0)),
	Step(Close_collect_no_cube(0.5)),
	Step(Background_lifter_to_preset(85.0, 0.0)),
	Step(Drive(true, -6.0)),
	Step(Rotate_back(-25.0, 10.0)),
    Step(Lifter_to_height(85.0)),
	eject,
	lifterToExch,
	AUTO_POSTAMBLE,
} ;

static Chain chain_two(steps_two, teleopex) ;
Executive right_scale_right(chain_two) ;

static vector<Step> steps_switch =
{
    AUTO_PREAMBLE("right_scale_right_switch_right"),
	Step(Background_lifter_to_preset(85.0, 0.0)),
	Step(Drive(253.0, 100.0, -30.0, false)),
    Step(Lifter_to_height(85.0)),
	Step(Eject(0.7)),
	lifterToFloor,
	Step(Rotate(-120)),
	Step(Drive_and_collect(56.0)),
	Step(Close_collect_no_cube(1.0)),
	Step(Drive(-6.0)),
	Step(Close_collect_no_cube(0.5)),
	lifterToSwitch,
	Step(Drive(24.0)),
    eject,
	Step(Drive(-6.0)),
	lifterToExch,
	AUTO_POSTAMBLE,
} ;

static Chain chain_switch(steps_switch, teleopex) ;
Executive right_scale_right_switch_right(chain_switch) ;

static vector<Step> steps_comp =
{
    AUTO_PREAMBLE("right_scale_right_comp"),
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

static Chain chain_comp(steps_comp, teleopex) ;
Executive right_scale_right_comp(chain_comp) ;
