#include "lifter_controller.h"
#include "message_logger.h"
#include "../subsystems.h"
#include <cmath>
#include <iostream>

using namespace std ;

LifterController::LifterController() {
    mMode = Mode::IDLE;
    mTarget = 0.0;
    mLastTarget = 0.0;
    mHeightThreshold = 0.0;
    mLastVoltage = 0.0;
    mMaxChange = 6.0;
    mDataDumpMode = false;
    mDataDumpStartTime = 0.0;
    mCalibrating = false ;
    mCalibrated = false ;
	mGear = Gear::High ;
}

bool LifterController::isCalibrated() const
{
    return mCalibrated ;
}

void LifterController::init(paramsInput* input_params) {
	paramsInput *p = paramsInput::get() ;
    mHeightThreshold = p->getValue("lifter:threshold", 1.0);
}

paramsInput* LifterController::getParams() {
    return mInputParams;
}

bool LifterController::nearPreset(Preset preset, double height, double tol)
{
    double preheight = presetToHeight(preset) ;

    return fabs(preheight - height) < tol ;
}

void LifterController::moveToHeight(double height, double current_height, double time) {
    mMode = Mode::HEIGHT;
    mTarget = height;
    mStartTime = time ;

    double p, i, d, f, imax;
	paramsInput *p = paramsInput::get() ;
    if(current_height < height) {
		p = mInputParams->getValue("lifter:up:p", 0.01);
		i = mInputParams->getValue("lifter:up:i", 0.0);
		d = mInputParams->getValue("lifter:up:d", 0.0);
		f = mInputParams->getValue("lifter:up:f", 0.0);
		imax = mInputParams->getValue("lifter:up:imax", 1000.0);
    } else {
		if (current_height - height > 6.0)
		{
			p = mInputParams->getValue("lifter:down:p", 0.01);
			i = mInputParams->getValue("lifter:down:i", 0.0);
			d = mInputParams->getValue("lifter:down:d", 0.0);
			f = mInputParams->getValue("lifter:down:f", 0.0);
			imax = mInputParams->getValue("lifter:down:imax", 1000.0);
		}
		else
		{
			p = mInputParams->getValue("lifter:downsmall:p", 0.01);
			i = mInputParams->getValue("lifter:downsmall:i", 0.0);
			d = mInputParams->getValue("lifter:downsmall:d", 0.0);
			f = mInputParams->getValue("lifter:downsmall:f", 0.0);
			imax = mInputParams->getValue("lifter:downsmall:imax", 1000.0);
		}
    }
	
    mHeightPID.Init(p, i, d, f, -0.8, 0.8, imax);

    messageLogger &logger = messageLogger::get();
    logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER_TUNING);
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

void LifterController::setCalibrate(bool calibrate) {
    if (mCalibrating == true && calibrate == false)
	mCalibrated = true ;
    
    mCalibrating = calibrate;
}

void LifterController::setManuallyAdjusted() {
	cout << "Setting manually adjusted" << endl; 
    mManuallyAdjusted = true;
}

void LifterController::udpateIdle(double time, double dt, double &out, Gear &gear)
{
	out = 0.0 ;
	gear = mGear ;
}

void LifterController::updateHeight(double time, double dt, double &out, Gear &gear)
{
	messageLogger &logger = messageLogger::get();
	
	logger.startMessage(messageLogger::messageType::debug, );
	if (std::fabs(mTarget - mCurrent) < mHeightThreshold)
	{
	    double elapsed= time - mStartTime ;
		logger << "Destination height reached in " ;
		mMode = Mode::IDLE ;
		mDataDumpMode = true;
		mDataDumpStartTime = time;
		
		updateIdle(time, dt, out, gear) ;
	}
	else
	{
		//
		// Get the output from the PID controller
		//
		out = mHeightPID.getOutput(mTarget, height, dt);

		//
		// Don't left the motors change votlage to quickly to prevent any
		// kind of brown out
		//
		double chg = std::fabs(out - mLastVoltage);
		if (chg > mMaxChange * dt)
		{
			if (out > mLastVoltage)
				out = mLastVoltage + mMaxChange * dt;
			else
				out = mLastVoltage - mMaxChange * dt;
		}
		mLastVoltage = out;
		
		logger.startMessage(messageLogger::messageType::debug);
		logger << "lifter mode HEIGHT";
		logger << ", time " << time;
		logger << ", dt "<< dt;
		logger << ", target " << mTarget;
		logger << ", height " << height;
		logger << ", out " << out;
		logger.endMessage() ;
	}
}
>>>>>>> Stashed changes

void LifterController::update(double height, double time, double dt, double& out, Gear &gear)
{
	mCurrent = height ;
	
	switch(mMode)
	{
	case Mode::IDLE:
		out = 0.0 ;
		gear = mGear ;
		break ;

	case Mode::HEIGHT:
		updateHeight(time, dt, out, gear) ;
		break ;

    if(mMode == Mode::IDLE) {
	out = 0.0;
    }
}

void LifterController::idle(double height, double time, double dt) {
    if (mDataDumpMode) {
	messageLogger &logger = messageLogger::get();
	logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER_TUNING);
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
	cout << "updateHeightOnChagne called" << endl ;
	if (mManuallyAdjusted)
	{
		cout << "manually adjusted in updateheight onchange" << endl ;
		cout << "height " << height << ", current_height " << current_height << endl ;
	}
    if (std::fabs(height - mLastTarget) > mHeightThreshold || mManuallyAdjusted) {
		moveToHeight(height, current_height, time);
		mLastTarget = height;
    }
}

void LifterController::updateHeightOnChange(Preset preset, double current_height, double time) {
    updateHeightOnChange(presetToHeight(preset), current_height, time);
}

bool LifterController::finishedTarget(double target) {
    bool ret = false ;

	if (mMode == Mode::IDLE && fabs(mTarget - target) < mHeightThreshold && !mManuallyAdjusted)
	{
		ret = true ;
	}

	return ret ;
}

bool LifterController::finishedTarget(Preset target) {
    return finishedTarget(presetToHeight(target));
}

bool LifterController::done() {
    return mMode == Mode::IDLE;
}

bool LifterController::runningInBackground() {
    return mMode == Mode::BACKGROUND;
}

bool LifterController::calibrating() {
    return mCalibrating;
}

double LifterController::presetToHeight(Preset preset) {
	double ret = 11.375 ;
	paramsInput *p = paramsInput::get() ;
	
    double floor_height = mInputParams->getValue("lifter:height:floor", 0.0);


	double exchange_height = mInputParams->getValue("lifter:height:exchange", 3.0);
    double drop_grabber_height = mInputParams->getValue("lifter:height:drop_grabber", 6.0);
    double switch_height = mInputParams->getValue("lifter:height:switch", 19.0);
    double scale_height = mInputParams->getValue("lifter:height:scale", 72.0);
    double prep_climb_height = mInputParams->getValue("lifter:height:prep_climb", 84.0);
	
    switch(preset) {
    case Preset::FLOOR:
		ret = p->getValue("lifter:height:floor", 0.0);
		break ;
		
    case Preset::EXCHANGE:
		return exchange_height;
    case Preset::DROP_GRABBER:
		return drop_grabber_height;
    case Preset::SWITCH: return switch_height;
    case Preset::SCALE: return scale_height;
    case Preset::PREP_CLIMB: return prep_climb_height;
    default: assert(0);
    }

	return ret ;
}
