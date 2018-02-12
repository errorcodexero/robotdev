#include "grabber_controller.h"
#include "message_logger.h"
#include <cmath>

GrabberController::GrabberController() {
    mMode = Mode::IDLE;
    mTarget = 0.0;
    mLastTarget = 0.0;
    mAngleThreshold = 0.0;
    mLastVoltage = 0.0;
    mMaxChange = 6.0;
    mDataDumpMode = false;
    mDataDumpStartTime = 0.0;
}

void GrabberController::setParams(paramsInput* input_params) {
    mInputParams = input_params;
    mAngleThreshold = mInputParams->getValue("grabber:threshold", 1.0);
}

void GrabberController::moveToAngle(double angle, double time) {
    mMode = Mode::ANGLE;
    mTarget = angle;
    mStartTime = time ;

    double p = mInputParams->getValue("grabber:p", 0.01);
    double i = mInputParams->getValue("grabber:i", 0.0);
    double d = mInputParams->getValue("grabber:d", 0.0);
    double f = mInputParams->getValue("grabber:f", 0.0);
    double imax = mInputParams->getValue("grabber:imax", 1000.0);
	
    mAnglePID.Init(p, i, d, f, -0.4, 0.4, imax);

    messageLogger &logger = messageLogger::get();
    logger.startMessage(messageLogger::messageType::debug);
    logger << "moveToAngle, angle = " << angle;
    logger << ", pid " << p << " " << i << " " << d << " " << f << " " << imax;
    logger.endMessage();
}

void GrabberController::moveToAngle(Preset preset, double time) {
    moveToAngle(presetToAngle(preset), time);
}

void GrabberController::update(double angle, double time, double dt, double& out) {
    messageLogger &logger = messageLogger::get();
	
    if(mMode == Mode::ANGLE) {
	if(std::fabs(mTarget - angle) < mAngleThreshold)
	    mMode = Mode::IDLE;

	out = mAnglePID.getOutput(mTarget, angle, dt);

	double chg = std::fabs(out - mLastVoltage);
	if (chg > mMaxChange * dt) {
	    if (out > mLastVoltage)
		out = mLastVoltage + mMaxChange * dt;
	    else
		out = mLastVoltage - mMaxChange * dt;
	}
	mLastVoltage = out;

	logger.startMessage(messageLogger::messageType::debug);
	logger << "grabber:update(ANGLE)";
	logger << ", time " << time;
	logger << ", dt "<< dt;
	logger << ", target " << mTarget;
	logger << ", angle " << angle;
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

void GrabberController::idle(double angle, double time, double dt) {
    if (mDataDumpMode) {
	messageLogger &logger = messageLogger::get();
	logger.startMessage(messageLogger::messageType::debug);
	logger << "IDLE: dt " << dt;
	logger << ", time " << time;
	logger << ", angle " << angle;
	logger.endMessage();

	if (time - mDataDumpStartTime > 5.0)
	    mDataDumpMode = false;
    }
}

void GrabberController::updateAngleOnChange(double angle, double time) {
    //
    // Updated so that we do not compare two floating point
    // numbers.  If a new angle asked for is within the threshold
    // we consider valid for reaching our target angle we do
    // not initialize for the new angle
    //
    if (std::fabs(angle - mLastTarget) > mAngleThreshold) {
	moveToAngle(angle, time);
	mLastTarget = angle;
    }
}

void GrabberController::updateAngleOnChange(Preset preset, double time) {
    updateAngleOnChange(presetToAngle(preset), time);
}

bool GrabberController::done() {
    return mMode == Mode::IDLE;
}

double GrabberController::presetToAngle(Preset preset) {
    double closed_angle = mInputParams->getValue("grabber:angle:closed", 0.0);
    double open_angle = mInputParams->getValue("grabber:angle:open", 30.0);
    double stowed_angle = mInputParams->getValue("grabber:angle:stowed", 90.0);

    switch(preset) {
    case Preset::CLOSED: return closed_angle;
    case Preset::OPEN: return open_angle;
    case Preset::STOWED: return stowed_angle;
    default: assert(0);
    }
}
