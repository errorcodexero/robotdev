#include "lifter_controller.h"
#include "message_logger.h"
#include <cmath>

LifterController::LifterController() {
	mMode = Mode::IDLE;
	mTarget = 0.0;
	mLastTarget = 0.0;
	mHeightThreshold = 0.0;
	mLastVoltage = 0.0;
	mMaxChange = 6.0;
	mDataDumpMode = false;
	mDataDumpStartTime = 0.0;
}

void LifterController::setParams(paramsInput* input_params) {
	mInputParams = input_params;
	mHeightThreshold = mInputParams->getValue("lifter:threshold", 1.0);
}

void LifterController::initHeight(double height) {
	mMode = Mode::HEIGHT;
	mTarget = height;

	double p = mInputParams->getValue("lifter:p", 0.01);
	double i = mInputParams->getValue("lifter:i", 0.0);
	double d = mInputParams->getValue("lifter:d", 0.0);
	double f = mInputParams->getValue("lifter:f", 0.0);
	double imax = mInputParams->getValue("lifter:imax", 1000.0);
	
	mHeightPID.Init(p, i, d, f, -0.4, 0.4, imax);

	messageLogger &logger = messageLogger::get();
	logger.startMessage(messageLogger::messageType::debug);
	logger << "initHeight, height = " << height;
	logger << ", pid " << p << " " << i << " " << d << " " << f << " " << imax;
	logger.endMessage();
}

void LifterController::initHeight(Preset preset) {
	initHeight(presetToHeight(preset));
}

void LifterController::update(double height, double time, double dt, double& out) {
	messageLogger &logger = messageLogger::get();
	
	if(mMode == Mode::HEIGHT) {
		if(std::fabs(mTarget - height) < mHeightThreshold)
			mMode = Mode::IDLE;

		out = mHeightPID.getOutput(mTarget, height, dt);

		double chg = std::fabs(out - mLastVoltage);
		if (chg > mMaxChange * dt) {
			if (out > mLastVoltage)
				out = mLastVoltage + mMaxChange * dt;
			else
				out = mLastVoltage - mMaxChange * dt;
		}
		mLastVoltage = out;

		logger.startMessage(messageLogger::messageType::debug);
		logger << "update(ANGLE)";
		logger << ", time " << time;
		logger << ", dt "<< dt;
		logger << ", target " << mTarget;
		logger << ", height " << height;
		logger << ", out " << out;
		if (mMode == Mode::IDLE)
			logger << ", REACHED";
		logger.endMessage();
	}

	if(mMode == Mode::IDLE) {
		out = 0.0;
		mDataDumpMode = true;
		mDataDumpStartTime = time;
	}
}

void LifterController::idle(double height, double time, double dt) {
	if (mDataDumpMode) {
		messageLogger &logger = messageLogger::get();
		logger.startMessage(messageLogger::messageType::debug);
		logger << "IDLE: dt " << dt;
		logger << ", time " << time;
		logger << ", height " << height;
		logger.endMessage();

		if (time - mDataDumpStartTime > 5.0)
			mDataDumpMode = false;
	}
}

void LifterController::initOnChange(double height) {
	if(height != mLastTarget) {
		initHeight(height);
		mLastTarget = height;
	}
}

void LifterController::initOnChange(Preset preset) {
	initOnChange(presetToHeight(preset));
}

bool LifterController::done() {
	return mMode == Mode::IDLE;
}

double LifterController::presetToHeight(Preset preset) {
	double floor_height = mInputParams->getValue("lifter:height:floor", 0.0);
	double exchange_height = mInputParams->getValue("lifter:height:exchange", 3.0);
	double switch_height = mInputParams->getValue("lifter:height:switch", 19.0);
	double scale_height = mInputParams->getValue("lifter:height:scale", 72.0);
	double prep_climb_height = mInputParams->getValue("lifter:height:prep_climb", 84.0);

	switch(preset) {
		case Preset::FLOOR: return floor_height;
		case Preset::EXCHANGE: return exchange_height;
		case Preset::SWITCH: return switch_height;
		case Preset::SCALE: return scale_height;
		case Preset::PREP_CLIMB: return prep_climb_height;
		default: assert(0);
	}
}
