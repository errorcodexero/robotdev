#include "drivebase_controller.h"
#include <cmath>

DrivebaseController::DrivebaseController() {
	mode = Mode::IDLE;
	zero_yaw = false;
	target = 0.0;
	distance_threshold = 0.0;
	angle_threshold = 0.0;
}

void DrivebaseController::setParams(paramsInput* input_params) {
	input_params = input_params;
	distance_threshold = input_params->getValue("drivebase:straight:threshold", 1.0);
	angle_threshold = input_params->getValue("drivebase:angle:threshold", 1.0);
}

void DrivebaseController::initDistance(double distance) {
	mode = Mode::DISTANCE;
	target = distance;
	dist_pid.Init(input_params->getValue("drivebase:distance:p", 0.2),
		input_params->getValue("drivebase:distance:i", 0.0),
		input_params->getValue("drivebase:distance:d", 0.0),
		input_params->getValue("drivebase:distance:f", 0.0),
		-1.0,
		1.0,
		input_params->getValue("drivebase:distance:imax", 1000.0)
	);
	straightness_pid.Init(input_params->getValue("drivebase:angleCorrection:p", 0.2),
		input_params->getValue("drivebase:angleCorrection:i", 0.0),
		input_params->getValue("drivebase:angleCorrection:d", 0.0),
		input_params->getValue("drivebase:angleCorrection:f", 0.0),
		-1.0,
		 1.0,
		input_params->getValue("drivebase:angleCorrection:imax", 1000.0)
	);
	zero_yaw = true;
}

void DrivebaseController::initAngle(double angle) {
	mode = Mode::ANGLE;
	target = angle;
	angle_pid.Init(input_params->getValue("drivebase:angle:p", 0.2),
		input_params->getValue("drivebase:angle:i", 0.0),
		input_params->getValue("drivebase:angle:d", 0.0),
		input_params->getValue("drivebase:angle:f", 0.0),
		-1.0,
		 1.0,
		input_params->getValue("drivebase:angle:imax", 1000.0)
	);
	zero_yaw = true;
}

void DrivebaseController::update(double distances_l, double distances_r, double angle, double dt, double& out_l, double& out_r, bool& out_zero_yaw) {
	out_zero_yaw = zero_yaw;
	zero_yaw = false;

	if(mode == Mode::DISTANCE) {
		double avg_dist = (distances_l + distances_r) / 2.0;
		if(std::fabs(target - avg_dist) < distance_threshold) {
			mode = Mode::IDLE;
		}

		double base = dist_pid.getOutput(0.0, target - avg_dist, dt);
		double offset = straightness_pid.getOutput(0.0, angle, dt);
		out_l = base + offset;
		out_r = base - offset;
	} else if(mode == Mode::ANGLE) {
		if(std::fabs(target - angle) < angle_threshold) {
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
}

bool DrivebaseController::done() {
	return mode == Mode::IDLE;
}
