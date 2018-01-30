#include "drivebase_controller.h"
#include "message_logger.h"
#include "UdpBroadcastSender.h"

#include <cmath>
#include <iostream>

int seq = 0 ;

DrivebaseController::DrivebaseController() {
	mode = Mode::IDLE;
	zero_yaw = false;
	target = 0.0;
	distance_threshold = 0.0;
	angle_threshold = 0.0;
	mLastVoltage = 0.0 ;
	mMaxChange = 3.0 ;

	mSender.open(8888) ;
}

void DrivebaseController::setParams(paramsInput* input_params) {
	mInput_params = input_params;
	distance_threshold = mInput_params->getValue("drivebase:straight:threshold", 1.0);
	angle_threshold = mInput_params->getValue("drivebase:angle:threshold", 1.0);
}

void DrivebaseController::initDistance(double distance) {
	
	mode = Mode::DISTANCE;
	target = distance;

	mSender.send("new") ;

	double p = mInput_params->getValue("drivebase:distance:p", 0.015) ;
	double i = mInput_params->getValue("drivebase:distance:i", 0.1) ;
	double d = mInput_params->getValue("drivebase:distance:d", 0.0) ;
	double f = mInput_params->getValue("drivebase:distance:f", 0.0) ;
	double imax = mInput_params->getValue("drivebase:distance:imax", 6.0) ;

	dist_pid.Init(p, i, d, f, -0.6, 0.6, imax) ;
	
	double ap = mInput_params->getValue("drivebase:angleCorrection:p", 0.0) ;
	double ai = mInput_params->getValue("drivebase:angleCorrection:i", 0.0) ;
	double ad = mInput_params->getValue("drivebase:angleCorrection:d", 0.0) ;
	double af = mInput_params->getValue("drivebase:angleCorrection:f", 0.0) ;
	double aimax = mInput_params->getValue("drivebase:angleCorrection:imax", 0.0) ;
	
	straightness_pid.Init(ap, ai, ad, af, -0.6, 0.6, aimax) ;
	zero_yaw = true;

	messageLogger &logger = messageLogger::get() ;
	logger.startMessage(messageLogger::messageType::debug) ;
	logger << "initDistance, distance = " << distance ;
	logger << ", dpid " << p << " " << i << " " << d << " " << f << " " << imax ;
	logger << ", apid " << ap << " " << ai << " " << ad << " " << af << " " << imax ;
	logger.endMessage() ;
}

void DrivebaseController::initAngle(double angle) {
	mode = Mode::ANGLE;
	target = angle;
	
	double ap = mInput_params->getValue("drivebase:angle:p", 0.0) ;
	double ai = mInput_params->getValue("drivebase:angle:i", 0.0) ;
	double ad = mInput_params->getValue("drivebase:angle:d", 0.0) ;
	double af = mInput_params->getValue("drivebase:angle:f", 0.0) ;
	double aimax = mInput_params->getValue("drivebase:angle:imax", 0.0) ;
	
	angle_pid.Init(ap, ai, ad, af, -0.6, 0.6, aimax) ;
	zero_yaw = true;

	messageLogger &logger = messageLogger::get() ;
	logger.startMessage(messageLogger::messageType::debug) ;
	logger << "initAngle, angle = " << angle ;
	logger << ", apid " << ap << " " << ai << " " << ad << " " << af << " " << aimax ;
	logger.endMessage() ;
}

void DrivebaseController::update(double distances_l, double distances_r, double angle, double dt, double time,
								 double& out_l, double& out_r, bool& out_zero_yaw)
{
	messageLogger &logger = messageLogger::get() ;
	std::string msg ;
	
	out_zero_yaw = zero_yaw;
	zero_yaw = false;

	if(out_zero_yaw)
	{
		logger.startMessage(messageLogger::messageType::debug) ;
		logger << "First cycle: settings YAW to zero" ;
		logger.endMessage() ;

		mLeftStart = distances_l ;
		mRightStart = distances_r ;
	}
	else
	{
		if(mode == Mode::DISTANCE) {
			double avg_dist = (distances_l + distances_r) / 2.0;
			if((target - avg_dist) < distance_threshold) {
				mode = Mode::IDLE;
			}

			msg = "data," + std::to_string(seq++) ;
			msg += ",time=" + std::to_string(time) ;
			msg += ",ldist=" + std::to_string(distances_l - mLeftStart) ;
			msg += ",rdist=" + std::to_string(distances_r - mRightStart)  ;
			double stavg = (mLeftStart + mRightStart) / 2.0 ;
			msg += ",dist=" + std::to_string(avg_dist - stavg) ;
			msg += ",angle=" + std::to_string(angle) ;
				
			double base = dist_pid.getOutput(target, avg_dist, dt);
			msg += ",base=" + std::to_string(angle) ;
			
			double chg = std::fabs(base - mLastVoltage) ;
			if (chg > mMaxChange * dt)
			{
				if (base > mLastVoltage)
					base = mLastVoltage + mMaxChange * dt ;
				else
					base = mLastVoltage - mMaxChange * dt ;
			}
			msg += ",change=" + std::to_string(angle) ;

			mLastVoltage = base ;
			double offset = straightness_pid.getOutput(0.0, angle, dt);
			out_l = base + offset;
			out_r = base - offset;
		
			msg += ",left=" + std::to_string(out_l) ;
			msg += ",right=" + std::to_string(out_r) ;

			mSender.send(msg) ;

			messageLogger &logger = messageLogger::get() ;
			logger.startMessage(messageLogger::messageType::debug) ;
			logger << "update(DISTANCE)" ;
			logger << ", angle " << angle ;
			logger << ", target " << target ;
			logger << ", distance " << avg_dist ;
			logger << ", ldist " << distances_l ;
			logger << ", rdist " << distances_r ;
			logger << ", base " << base ;
			logger << ", offset " << offset ;
			logger << ", l " << out_l ;
			logger << ", r " << out_r ;
			if (mode == Mode::IDLE)
				logger << ", TARGET" ;
			logger.endMessage() ;
	
			
		} else if(mode == Mode::ANGLE) {
			if((target - angle) < angle_threshold) {
				mode = Mode::IDLE;
			}
	
			double base = angle_pid.getOutput(target, angle, dt);
			out_l = base;
			out_r = -base;
	
			messageLogger &logger = messageLogger::get() ;
			logger.startMessage(messageLogger::messageType::debug) ;
			logger << "update(ANGLE)" ;
			logger << ", target " << target ;
			logger << ", angle " << angle ;
			logger << ", base " << base ;
			logger << ", l " << out_l ;
			logger << ", r " << out_r ;
			if (mode == Mode::IDLE)
				logger << ", TARGET" ;
			logger.endMessage() ;
		}
	}
	
	if(mode == Mode::IDLE) {
		out_l = 0.0;
		out_r = 0.0;
		zero_yaw = true ;
	}
}

bool DrivebaseController::done() {
	return mode == Mode::IDLE;
}
