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
	lifterToFloor,
	startLifterExch,
#ifdef SAFE_TWO_CUBE
	Step(Drive(290, false)),
	Step(Rotate(90, 8.0)),
    Step(Lifter_to_height(85.0)),
    eject,
	lifterToFloor,
    Step(Rotate_finish(90, 67, 3.0)),
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
#else
	Step(Background_lifter_to_preset(85.0, 0.0)),
	Step(Drive(250.0, 100.0, 25.0, false)),
    Step(Lifter_to_height(85.0)),
	Step(Eject(0.4)),
	lifterToFloor,
	Step(Rotate(120)),
	Step(Drive_and_collect(61.0)),
	Step(Close_collect_no_cube(1.0)),
	Step(Drive(-6.0)),
	Step(Close_collect_no_cube(0.5)),
	Step(Background_lifter_to_preset(85.0, 0.0)),
	Step(Drive(true, -6.0)),
	Step(Rotate_back(20.0, 10.0)),
    Step(Lifter_to_height(85.0)),
	eject,
	lifterToExch,
#endif
	AUTO_POSTAMBLE,
} ;

static Chain chain(steps, teleopex) ;
Executive left_scale_left(chain) ;

static vector<Step> steps_switch =
{
    AUTO_PREAMBLE("left_scale_left_switch_left"),
	lifterToFloor,
	startLifterExch,	
#ifdef SAFE_TWO_CUBE
	Step(Drive(290, false)),
	Step(Rotate(90, 8.0)),
    Step(Lifter_to_height(85.0)),
    eject,
	lifterToFloor,
    Step(Rotate_finish(90, 67, 3.0)),
	Step(Drive_and_collect(114.0)),
	Step(Close_collect_no_cube(1.0)),
	Step(Drive(-6.0)),
	Step(Close_collect_no_cube(0.5)),
	lifterToSwitch,
	Step(Drive(12.0)),
    eject,
	Step(Drive(-6.0)),
	lifterToExch,
#else
	Step(Background_lifter_to_preset(85.0, 0.0)),
	Step(Drive(250.0, 100.0, 25.0, false)),
    Step(Lifter_to_height(85.0)),
	Step(Eject(0.4)),
	lifterToFloor,
	Step(Rotate(115)),
	Step(Drive_and_collect(60.0)),
	Step(Close_collect_no_cube(1.0)),
	Step(Drive(-6.0)),
	Step(Close_collect_no_cube(0.5)),
	lifterToSwitch,
	Step(Drive(24.0)),
    eject,
	Step(Drive(-6.0)),
	lifterToExch,
#endif
	AUTO_POSTAMBLE,
} ;
static Chain chain_switch(steps_switch, teleopex) ;
Executive left_scale_left_switch_left(chain_switch) ;
