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

paramsInput* LifterController::getParams() {
    return mInputParams;
}

void LifterController::moveToHeight(double height, double current_height, double time) {
    mMode = Mode::HEIGHT;
    mTarget = height;
    mStartTime = time ;

    double p, i, d, f, imax;
    if(current_height < height) {
	p = mInputParams->getValue("lifter:up:p", 0.01);
	i = mInputParams->getValue("lifter:up:i", 0.0);
	d = mInputParams->getValue("lifter:up:d", 0.0);
	f = mInputParams->getValue("lifter:up:f", 0.0);
	imax = mInputParams->getValue("lifter:up:imax", 1000.0);
    } else {
	p = mInputParams->getValue("lifter:down:p", 0.01);
	i = mInputParams->getValue("lifter:down:i", 0.0);
	d = mInputParams->getValue("lifter:down:d", 0.0);
	f = mInputParams->getValue("lifter:down:f", 0.0);
	imax = mInputParams->getValue("lifter:down:imax", 1000.0);
    }
	
    mHeightPID.Init(p, i, d, f, -0.4, 0.4, imax);

    messageLogger &logger = messageLogger::get();
    logger.startMessage(messageLogger::messageType::debug);
    logger << "moveToHeight, height = " << height;
    logger << ", pid " << p << " " << i << " " << d << " " << f << " " << imax;
    logger.endMessage();
}

void LifterController::moveToHeight(Preset preset, double current_height, double time) {
    moveToHeight(presetToHeight(preset), current_height, time);
}

void LifterController::backgroundMoveToHeight(Preset preset, double current_height, double time) {
    moveToHeight(presetToHeight(preset), current_height, time);
    mMode = Mode::BACKGROUND;
}

void LifterController::calibrate() {
    mMode = Mode::CALIBRATING;
}

void LifterController::update(double height, double time, double dt, double& out) {
    messageLogger &logger = messageLogger::get();
	
    if(mMode == Mode::HEIGHT || mMode == Mode::BACKGROUND) {
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
	logger << "lifter:update(HEIGHT)";
	logger << ", time " << time;
	logger << ", dt "<< dt;
	logger << ", target " << mTarget;
	logger << ", height " << height;
	logger << ", out " << out;
	if (mMode == Mode::IDLE)
	{
	    double elapsed= time - mStartTime ;
	    logger << ", SUCCESS , " << elapsed << " seconds" ;
	}
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

void LifterController::updateHeightOnChange(double height, double current_height, double time) {
    //
    // Updated so that we do not compare two floating point
    // number.  If a new height asked for is within the threshold
    // we consider valid for reaching our target height we do
    // not initialize for the new height
    //
    if (std::fabs(height - mLastTarget) > mHeightThreshold) {
	moveToHeight(height, current_height, time);
	mLastTarget = height;
    }
}

void LifterController::updateHeightOnChange(Preset preset, double current_height, double time) {
    updateHeightOnChange(presetToHeight(preset), current_height, time);
}

bool LifterController::done() {
    return mMode == Mode::IDLE;
}

bool LifterController::runningInBackground() {
    return mMode == Mode::BACKGROUND;
}

bool LifterController::calibrating() {
    return mMode == Mode::CALIBRATING;
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
