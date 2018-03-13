#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: N/A
// Action: Test the steps for collecting a cube and driving back
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps =
{
	Step(StartAuto("collect_test")),
	calibrateLifter,
	calibrateGrabber,
	eject,
	Step(Wait(2.0)),
	driveAndCollect,
	Step(Wait(1.0)),
	driveBackFromCollect,
	endAuto
};

static Chain chain(steps, teleopex);
Executive collect_test(chain);
