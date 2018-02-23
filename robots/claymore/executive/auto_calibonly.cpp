#include "autosteps.h"
#include <vector>

using namespace std ;

////////////////////   Calibrate Only Auto Program /////////////////////////////////

vector<Step> calibrate_only_steps =
{
    startAuto,
    calibrateLifter,
    calibrateGrabber,
    endAuto
} ;

Chain calibrate_only_chain(calibrate_only_steps, teleopex) ;

Executive calibrate_only(calibrate_only_chain) ;

