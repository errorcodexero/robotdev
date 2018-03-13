#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: N/A
// Action: Test the step for collecting a cube
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps =
{
	Step(StartAuto("collect_test")),
	calibrateLifter,
	calibrateGrabber,
	eject,
	Step(Wait(2.0)),
	Step(Drive_and_collect()),
	endAuto
};

static Chain chain(steps, teleopex);
Executive collect_test(chain);
