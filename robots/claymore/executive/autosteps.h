#include "step.h"
#include "drive.h"
#include "rotate.h"
#include "rotateback.h"
#include "startauto.h"
#include "endauto.h"
#include "waitstep.h"
#include "lifterstep.h"
#include "calibratelifter.h"
#include "calibrategrabber.h"
#include "drivecollect.h"
#include "eject.h"
#include "teleop.h"

//
// I don't really like this, but it fixes the problem.  We will do this better next
// year with the new architecture
//

static Step endAuto = Step(EndAuto()) ;
static Step calibrateLifter = Step(Calibrate_lifter()) ;
static Step calibrateGrabber = Step(Calibrate_grabber()) ;
static Step rotate90pos = Step(Rotate(90.0)) ;
static Step rotate90neg = Step(Rotate(-90.0)) ;
static Step startLifterFloor = Step(LifterStep(LifterController::Preset::FLOOR, true)) ;
static Step startLifterExch = Step(LifterStep(LifterController::Preset::EXCHANGE, true)) ;
static Step startLifterSwitch = Step(LifterStep(LifterController::Preset::SWITCH, true)) ;
static Step startLifterScale = Step(LifterStep(LifterController::Preset::SCALE, true)) ;
static Step lifterToFloor = Step(LifterStep(LifterController::Preset::FLOOR)) ;
static Step lifterToExch = Step(LifterStep(LifterController::Preset::EXCHANGE)) ;
static Step lifterToSwitch = Step(LifterStep(LifterController::Preset::SWITCH)) ;
static Step lifterToScale = Step(LifterStep(LifterController::Preset::SCALE)) ;
static Step eject = Step(Eject()) ;
static Step driveAndCollect = Step(Drive_and_collect());
static Step driveBackFromCollect = Step(Drive(true));

static Step lifterToExchFail = Step(lifterToExch, {
	Step(Drive("auto:c_switch_r:segment1", 105, true)),
	endAuto
});

#define AUTO_PREAMBLE(name)												\
	Step(StartAuto(name)),												\
	calibrateLifter,													\
	calibrateGrabber,													\
	startLifterExch

		
#define AUTO_POSTAMBLE \
	endAuto

//
// Executive used to end each automode step
//
static Executive teleopex{Teleop()} ;
