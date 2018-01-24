#include "RobotGPG3Consts.h"
#include <cstdint>

namespace RobotGPG3
{
	/////////////////////////////////////////////////////////////
	//
	// Physical Robot Constants
	//
	/////////////////////////////////////////////////////////////


	//
	// The number of pulses the encoder ticks per revolution of
	// a wheel.
	//
	const int PulsesPerRevolution = 1;

	//
	// The diameter of the wheels of the robot, in inches
	//
	const double WheelDiameter = 1;

	//
	// The maximum curising speed of the robot (inches per second)
	//
	const double MaxSpeed = 1;

	//
	// The maximum acceleration of the robot (inches per sec per sec)
	//
	const double MaxRobotAcceleration = 1 ;

	//
	// The maximum acceleration of the robot (inches per sec per sec)
	//
	const double MaxRobotDeceleration = 1;

	/////////////////////////////////////////////////////////////
	//
	// Constants the control the behavior of the drie base
	//
	/////////////////////////////////////////////////////////////

	//
	// The default P constant for the PID controller
	//
	const double DriveBaseP = 1.0;

	//
	// The default I constant for the PID controller
	//
	const double DriveBaseI = 1.0;

	//
	// The default D constant for the PID controller
	//
	const double DriveBaseD = 1.0;

	//
	// The time between evaluations of the drive base PID loop
	//
	const double DriveBaseSampleTime = 0.005;

	//
	// The life time of a speed profile.  When the speed profile is older
	// than this value in seconds, it is recalculated
	//
	const double DriveBaseSpeedProfileLifeTime = 1000 ;

	//
	// The number of speed samples to take into account to calculate speed
	//
	const int32_t SpeedSamples = 2;

	/////////////////////////////////////////////////////////////
	//
	// Constants the control the behavior of the system
	//
	/////////////////////////////////////////////////////////////

	//
	// The maximum time in milliseconds we wait for the NavX to complete
	// calibration
	//
	const int64_t MaxNavXCalibrationTime = 2000 ;
}
