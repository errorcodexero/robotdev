#pragma once

#include <cstdint>

namespace Robot2018
{
	//
	// The number of pulses the encoder ticks per revolution of
	// a wheel.
	//
	extern const int PulsesPerRevolution ;

	//
	// The diameter of the wheels of the robot, in inches
	//
	extern const double WheelDiameter ;

	//
	// The maximum curising speed of the robot (inches per second)
	//
	extern const double MaxSpeed ;

	//
	// The maximum acceleration of the robot (inches per sec per sec)
	//
	extern const double MaxRobotAcceleration ;

	//
	// The maximum acceleration of the robot (inches per sec per sec)
	//
	extern const double MaxRobotDeceleration ;

	//
	// The maximum time we wait for the NavX for calibration, in milliseconds
	//
	extern const int64_t MaxNavXCalibrationTime;

	//
	// The P constants for the PID controller
	//
	extern const double DriveBaseP;

	//
	// The I constants for the PID controller
	//
	extern const double DriveBaseI;

	//
	// The D constants for the PID controller
	//
	extern const double DriveBaseD;

	//
	// The time between samples when processing drive base
	// data
	//
	extern const double DriveBaseSampleTime;

	//
	// The age of a speed profile before a new profile is calculated
	//
	extern const double DriveBaseSpeedProfileLifeTime;

	//
	// The number of samples to average to calculate speed of the robot
	//
	extern const int32_t SpeedSamples;
}
