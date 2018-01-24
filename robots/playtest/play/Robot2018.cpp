#include "Robot2018.h"
#include "NavXAngleMeasurementDevice.h"
#include "EncoderAngleMeasurementDevice.h"
#include "MessageLogger.h"
#include <ctre/phoenix/MotorControl/CAN/WPI_TalonSRX.h>
#include <Encoder.h>
#include "AHRS.h"
#include "ParamFileReader.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace xerolib;

Robot2018::Robot2018()
{
}

Robot2018::~Robot2018()
{
}

void Robot2018::RobotInit()
{
    setupLoggers("robot.log", "robot.csv");
    setupConsoleLogger();
    setupSmartDashboardLogger() ;
    // setupTcpLogger("10.14.25.25", 8888);
    //setupUdpLogger(8888) ;
}

START_ROBOT_CLASS(Robot2018);
