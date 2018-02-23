#include "step.h"
#include "teleop.h"

//
// I don't really like this, but it fixes the problem.  We will do this better next
// year with the new architecture
//

static Step startAuto = Step(StartAuto()) ;
static Step endAuto = Step(EndAuto()) ;
static Step calibrateLifter = Step(Calibrate_lifter()) ;
static Step calibrateGrabber = Step(Calibrate_grabber()) ;
static Step rotate90pos = Step(Rotate(90.0)) ;
static Step rotate90neg = Step(Rotate(-90.0)) ;
static Step startLifterFloor = Step(Background_lifter_to_preset(LifterController::Preset::FLOOR, 0.0)) ;
static Step startLifterExch = Step(Background_lifter_to_preset(LifterController::Preset::EXCHANGE, 0.0)) ;
static Step startLifterSwitch = Step(Background_lifter_to_preset(LifterController::Preset::SWITCH, 0.0)) ;
static Step startLifterScale = Step(Background_lifter_to_preset(LifterController::Preset::SCALE, 0.0)) ;
static Step lifterToFloor = Step(Lifter_to_preset(LifterController::Preset::FLOOR, 0.0)) ;
static Step lifterToExch = Step(Lifter_to_preset(LifterController::Preset::EXCHANGE, 0.0)) ;
static Step lifterToSwitch = Step(Lifter_to_preset(LifterController::Preset::SWITCH, 0.0)) ;
static Step lifterToScale = Step(Lifter_to_preset(LifterController::Preset::SCALE, 0.0)) ;
static Step waitForLifter = Step(Wait_for_lifter()) ;
static Step eject = Step(Eject()) ;

//
// Executive used to end each automode step
//
static Executive teleopex{Teleop()} ;
