#include "autosteps.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Center
// Action: Score on left switch
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> grabber_test_steps
{
    startAuto,
    calibrateLifter,
    calibrateGrabber,
    endAuto,
};

static Chain grabber_test_chain(grabber_test_steps, teleopex) ;
Executive grabber_test(grabber_test_chain) ;
