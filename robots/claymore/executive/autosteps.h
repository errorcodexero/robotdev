#include "autonomous.h"
#include "teleop.h" 
#include "chain.h"
#include "step.h"
#include "message_logger.h"

extern Executive teleopex;

extern Step startAuto ;
extern Step endAuto ;
extern Step calibrateLifter ;
extern Step calibrateGrabber ;
extern Step rotate90pos ;
extern Step rotate90neg ;
extern Step startLifterFloor ;
extern Step startLifterExch ;
extern Step startLifterSwitch ;
extern Step startLifterScale ;
extern Step lifterToFloor ;
extern Step lifterToExch ;
extern Step lifterToSwitch ;
extern Step lifterToScale ;
extern Step waitForLifter ;
extern Step eject ;
