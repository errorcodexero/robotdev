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
	
	mode = Mode::DISTANCE;
	target = distance;

	double p = mInput_params->getValue("drivebase:distance:p", 0.015) ;
	double i = mInput_params->getValue("drivebase:distance:i", 0.1) ;
	double d = mInput_params->getValue("drivebase:distance:d", 0.0) ;
	double f = mInput_params->getValue("drivebase:distance:f", 0.0) ;
	double imax = mInput_params->getValue("drivebase:distance:imax", 6.0) ;

	dist_pid.Init(p, i, d, f, -1.0, 1.0, imax) ;
	
	double ap = mInput_params->getValue("drivebase:angleCorrection:p", 0.0) ;
	double ai = mInput_params->getValue("drivebase:angleCorrection:i", 0.0) ;
	double ad = mInput_params->getValue("drivebase:angleCorrection:d", 0.0) ;
	double af = mInput_params->getValue("drivebase:angleCorrection:f", 0.0) ;
	double aimax = mInput_params->getValue("drivebase:angleCorrection:imax", 0.0) ;
	
	straightness_pid.Init(ap, ai, ad, af, -1.0, 1.0, aimax) ;
	zero_yaw = true;

#ifdef _DEBUG
	std::cout << "initDistance, distance = " << distance << std::endl ;
	std::cout << ", dpid " << p << " " << i << " " << d << " " << f << " " << imax ;
	std::cout << ", apid " << ap << " " << ai << " " << ad << " " << af << " " << imax ;
	std::cout << std::endl ;
#endif	
}

void DrivebaseController::initAngle(double angle) {
	mode = Mode::ANGLE;
	target = angle;
	
	double ap = mInput_params->getValue("drivebase:angle:p", 0.0) ;
	double ai = mInput_params->getValue("drivebase:angle:i", 0.0) ;
	double ad = mInput_params->getValue("drivebase:angle:d", 0.0) ;
	double af = mInput_params->getValue("drivebase:angle:f", 0.0) ;
	double aimax = mInput_params->getValue("drivebase:angle:imax", 0.0) ;
	
	angle_pid.Init(ap, ai, ad, af, -1.0, 1.0, aimax) ;
	zero_yaw = true;

#ifdef _DEBUG	
	std::cout << "initAngle, angle = " << angle << std::endl ;
	std::cout << ", apid " << ap << " " << ai << " " << ad << " " << af << " " << aimax ;
	std::cout << std::endl ;
#endif
}

void DrivebaseController::update(double distances_l, double distances_r, double angle, double dt,
								 double& out_l, double& out_r, bool& out_zero_yaw)
{
	out_zero_yaw = zero_yaw;
	zero_yaw = false;

	if(mode == Mode::DISTANCE) {
		double avg_dist = (distances_l + distances_r) / 2.0;
		if((target - avg_dist) < distance_threshold) {
			mode = Mode::IDLE;
		}

		double base = dist_pid.getOutput(target, avg_dist, dt);
		double offset = straightness_pid.getOutput(0.0, angle, dt);
		out_l = base + offset;
		out_r = base - offset;
		
#ifdef _DEBUG
		std::cout << "update(DISTANCE)" ;
		std::cout << ", target " << target ;
		std::cout << ", distance " << avg_dist ;
		std::cout << ", base " << base ;
		std::cout << ", offset " << offset ;
		std::cout << ", l " << out_l ;
		std::cout << ", r " << out_r ;
		if (mode == Mode::IDLE)
			std::cout << ", TARGET" ;
		std::cout << std::endl ;
		
#endif
		
	} else if(mode == Mode::ANGLE) {
		if((target - angle) < angle_threshold) {
			mode = Mode::IDLE;
		}

		double base = angle_pid.getOutput(target, angle, dt);
		out_l = base;
		out_r = -base;
#ifdef _DEBUG
		std::cout << "update(ANGLE)" ;
		std::cout << ", target " << target ;
		std::cout << ", angle " << angle ;
		std::cout << ", base " << base ;
		std::cout << ", l " << out_l ;
		std::cout << ", r " << out_r ;
		if (mode == Mode::IDLE)
			std::cout << ", TARGET" ;
		std::cout << std::endl ;
#endif
	}

	if(mode == Mode::IDLE) {
		out_l = 0.0;
		out_r = 0.0;
	}
}

bool DrivebaseController::done() {
	return mode == Mode::IDLE;
}
