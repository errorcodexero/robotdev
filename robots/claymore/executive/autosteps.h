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

static EndAuto endAuto ;
static Calibrate_lifter calibrateLifter ;
static Calibrate_grabber calibrateGrabber ;
static Rotate rotate90pos(90.0) ;
static Rotate rotate90neg(-90) ;
static LifterStep startLifterFloor(LifterController::Preset::FLOOR, true) ;
static LifterStep startLifterExch(LifterController::Preset::EXCHANGE, true) ;
static LifterStep startLifterSwitch(LifterController::Preset::SWITCH, true) ;
static LifterStep startLifterScale(LifterController::Preset::SCALE, true) ;
static LifterStep lifterToFloor(LifterController::Preset::FLOOR) ;
static LifterStep lifterToExch(LifterController::Preset::EXCHANGE) ;
static LifterStep lifterToSwitch(LifterController::Preset::SWITCH) ;
static LifterStep lifterToScale(LifterController::Preset::SCALE) ;
static Eject eject ;
static Drive_and_collect driveAndCollect ;
static Drive driveBackFromCollect(true) ;

#define AUTO_PREAMBLE(name)					\
	calibrateLifter,						\
	calibrateGrabber,						\
	startLifterExch


#define AUTO_PREAMBLE_x(name)					\
	StartAuto(name),						\
	calibrateLifter,						\
	calibrateGrabber,						\
	startLifterExch

		
#define AUTO_POSTAMBLE \
	endAuto

//
// Executive used to end each automode step
//
static Executive teleopex{Teleop()} ;
