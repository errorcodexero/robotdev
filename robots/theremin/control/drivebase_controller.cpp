#include "drivebase_controller.h"
#include <cmath>
#include <iostream>

DrivebaseController::DrivebaseController() {
	mode = Mode::IDLE;
	zero_yaw = false;
	target = 0.0;
	distance_threshold = 0.0;
	angle_threshold = 0.0;
}

void DrivebaseController::setParams(paramsInput* input_params) {
	mInput_params = input_params;
	distance_threshold = mInput_params->getValue("drivebase:straight:threshold", 1.0);
	angle_threshold = mInput_params->getValue("drivebase:angle:threshold", 1.0);
}

void DrivebaseController::initDistance(double distance) {
	std::cout << "DriveBaseController::initDistance, distance = " << distance << std::endl ;
	
	mode = Mode::DISTANCE;
	target = distance;

	double p = mInput_params->getValue("drivebase:distance:p", 0.015) ;
	double i = mInput_params->getValue("drivebase:distance:i", 0.1) ;
	double d = mInput_params->getValue("drivebase:distance:d", 0.0) ;
	double f = mInput_params->getValue("drivebase:distance:f", 0.0) ;
	double imax = mInput_params->getValue("drivebase:distance:imax", 6.0) ;

	std::cout << "InitPID " << p << " " << i << " " << d << " " << f ;
	std::cout << " " << imax << std::endl ;
	
	dist_pid.Init(p, i, d, f, -1.0, 1.0, imax) ;
	
	straightness_pid.Init(mInput_params->getValue("drivebase:angleCorrection:p", 0.015),
		mInput_params->getValue("drivebase:angleCorrection:i", 0.1),
		mInput_params->getValue("drivebase:angleCorrection:d", 0.0),
		mInput_params->getValue("drivebase:angleCorrection:f", 0.0),
		-1.0,
		 1.0,
		mInput_params->getValue("drivebase:angleCorrection:imax", 1000.0)
	);
	zero_yaw = true;
}

void DrivebaseController::initAngle(double angle) {
	mode = Mode::ANGLE;
	target = angle;
	angle_pid.Init(mInput_params->getValue("drivebase:angle:p", 0.2),
		mInput_params->getValue("drivebase:angle:i", 0.0),
		mInput_params->getValue("drivebase:angle:d", 0.0),
		mInput_params->getValue("drivebase:angle:f", 0.0),
		-1.0,
		 1.0,
		mInput_params->getValue("drivebase:angle:imax", 1000.0)
	);
	zero_yaw = true;
}

void DrivebaseController::update(double distances_l, double distances_r, double angle, double dt,
								 double& out_l, double& out_r, bool& out_zero_yaw)
{
	std::cout << "update" ;
	out_zero_yaw = zero_yaw;
	zero_yaw = false;

	if(mode == Mode::DISTANCE) {
		double avg_dist = (distances_l + distances_r) / 2.0;
		std::cout << ", distance " << avg_dist ;
		if((target - avg_dist) < distance_threshold) {
			std::cout << "AT TARGET" ;
			mode = Mode::IDLE;
		}

		double remaining = target - avg_dist ;
		double base = dist_pid.getOutput(0.0, remaining, dt);
		std::cout << ", remaining " << remaining ;
		std::cout << ", base " << base ;
		double offset = straightness_pid.getOutput(0.0, angle, dt);
		std::cout << ", offset " << offset ;
		out_l = base + offset;
		out_r = base - offset;
		std::cout << ", l " << out_l ;
		std::cout << ", r " << out_r ;
		
	} else if(mode == Mode::ANGLE) {
		if((target - angle) < angle_threshold) {
			mode = Mode::IDLE;
		}

		double base = angle_pid.getOutput(target, angle, dt);
		out_l = base;
		out_r = -base;
	}

	if(mode == Mode::IDLE) {
		out_l = 0.0;
		out_r = 0.0;
	}
	std::cout << std::endl ;
}

bool DrivebaseController::done() {
	return mode == Mode::IDLE;
}
