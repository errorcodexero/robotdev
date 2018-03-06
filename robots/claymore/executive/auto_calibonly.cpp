#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

////////////////////   Calibrate Only Auto Program /////////////////////////////////

static vector<Step> steps =
{
    Step(StartAuto("calibrate_only")),
    calibrateLifter,
    calibrateGrabber,
    endAuto
} ;

static Chain chain(steps, teleopex) ;

Executive calibrate_only(chain) ;

