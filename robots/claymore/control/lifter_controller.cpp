#include "lifter_controller.h"
#include "message_logger.h"
#include "../subsystems.h"
#include <cmath>
#include <iostream>

using namespace std ;

//
// The nubmer of inches per tick for the lifter
//
const double LifterController::INCHES_PER_TICK_HIGH_GEAR = .08327 ;

//
// The height of the top of the collector in inches
//
const double LifterController::COLLECTOR_OFFSET = 11.375;


LifterController::LifterController()
{
    mMode = Mode::IDLE;
    mTarget = 11.375 ;
	mCurrent = 11.375 ;
	mLastHeight = 11.375 ;
    mLastTarget = 11.375 ;
	
    mHeightThreshold = 0.0;
    mLastVoltage = 0.0;
    mMaxChange = 6.0;
	
    mDataDumpMode = false;
    mDataDumpStartTime = 0.0;
	
    mCalibrated = false ;
	mGear = Gear::High ;
}

void LifterController::init(paramsInput* input_params)
{
	paramsInput *p = paramsInput::get() ;
    mHeightThreshold = p->getValue("lifter:threshold", 1.0);
}

void LifterController::moveToHeight(double height, double time)
{
	if (std::fabs(height - mCurrent) > mHeightThreshold)
	{
		double p, i, d, f, imax;
		double vmin, vmax ;
		paramsInput *p = paramsInput::get() ;
		double smallthresh = p->getValue("lifter:downsmall:threshold") ;
		double delta = mHeight - height ;

		if (mMode != Mode::HEIGHT || std::fabs(mTarget - height) > mHeightThreshold)
		{
			//
			// This is a new request, or our target has changed, restart our idea of
			// when the move request was initiated
			//
			mStartTime = time ;
		}
		
		mMode = Mode::HEIGHT;
		mTarget = height;
		
		if(current_height < height)
		{
			p = p->getValue("lifter:up:p", 0.01);
			i = p->getValue("lifter:up:i", 0.0);
			d = p->getValue("lifter:up:d", 0.0);
			f = p->getValue("lifter:up:f", 0.0);
			imax = p->getValue("lifter:up:imax", 1000.0);
			vmin = p->getValue("lifter:up:vmin", -0.8) ;
			vmax = p->getValue("lifter:up:vmax", 0.8) ;
		}
		else if (current_height >= height && delta > smallthresh)
		{
			p = p->getValue("lifter:down:p", 0.01);
			i = p->getValue("lifter:down:i", 0.0);
			d = p->getValue("lifter:down:d", 0.0);
			f = p->getValue("lifter:down:f", 0.0);
			imax = p->getValue("lifter:down:imax", 1000.0);
			vmin = p->getValue("lifter:down:vmin", -0.8) ;
			vmax = p->getValue("lifter:down:vmax", 0.8) ;
		}
		else
		{
			p = p->getValue("lifter:downsmall:p", 0.01);
			i = p->getValue("lifter:downsmall:i", 0.0);
			d = p->getValue("lifter:downsmall:d", 0.0);
			f = p->getValue("lifter:downsmall:f", 0.0);
			imax = p->getValue("lifter:downsmall:imax", 1000.0);
			vmin = p->getValue("lifter:downsmall:vmin", -0.8) ;
			vmax = p->getValue("lifter:downsmall:vmax", 0.8) ;
		}
		
		mHeightPID.Init(p, i, d, f, vmin, vmax, imax);
		
		messageLogger &logger = messageLogger::get();
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER_TUNING);
		logger << "moveToHeight, height = " << height;
		logger << ", pid " << p << " " << i << " " << d << " " << f << " " << imax;
		logger << ", vmin " << vmin, << ", vmax " << vmax ;
		logger.endMessage();
	}
	else
	{
		messageLogger &logger = messageLogger::get();
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER);
		logger << "moveToHeight, target = " << height ;
		logger << ", current = " << mCurrent ;
		logger << ", threshold = " << mHeightThreshold ;
		logger << ", REQUEST IGNORED" ;
		logger.endMessage();
	}
}

void LifterController::moveToHeight(Preset preset, double current_height, double time)
{
    moveToHeight(presetToHeight(preset), current_height, time);
}

void LifterController::updateIdle(double time, double dt, double &out, Gear &gear, bool &brake)
{
	paramsInput *p = paramsInput::get() ;

	if (p->getValue("lifter:idle:use_brake", 0.0) > 0.5)
	{
		out = 0.0 ;
		gear = mGear ;
		brake = true ;
	}
	else
	{
		if (mGear == Gear::LOW)
			out = p->getValue("lifter:hold_power:low_gear", 0.0) ;
		else
			out = p->getValue("lifter:hold_power:high_gear", 0.1) ;
		
		gear = mGear ;
	}
	
    if (mDataDumpMode) {
		messageLogger &logger = messageLogger::get();
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER_TUNING);
		logger << "lifter MODE idle, dt " << dt;
		logger << ", time " << time;
		logger << ", height " << mCurrent ;
		logger << ", ticks " << mTicks ;
		logger.endMessage();
		
		if (time - mDataDumpStartTime > 5.0)
			mDataDumpMode = false;
    }
}

//
// updateCalibrate just runs for a single robot loop and we record the number of ticks
// that we see a the bottom of the lift travel.  This should only be called when
// the lift is at the bottom of travel.
//
void LifterController::udpateCalibrate(double time, double dt, double &out, Gear &gear, bool &brake)
{
	out = 0.0 ;
	gear = mGear ;

	mBaseTicks = mTicks ;
	mCalibrated = true ;

	calcHeight() ;
	mLastHeight = mCurrent ;

	mMode = Mode::IDLE ;
	
	messageLogger &logger = messageLogger::get();
	logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER);
	logger << "Lifter calibrated at " << mBaseTicks << " ticks" ;
	logger.endMessage() ;
}

//
// updateHeight is called when the mode is HEIGHT mode.  It drives the lift up or down
// as necessare to move the lift to the target height
//
void LifterController::updateHeight(double time, double dt, double &out, Gear &gear, bool &brake)
{
	messageLogger &logger = messageLogger::get();

	//
	// If the lift is not calibreated, we cannot trust heights so we don't move to
	// heights
	//
	if (mCalibrated)
	{
		if (std::fabs(mTarget - mCurrent) < mHeightThreshold)
		{
			double elapsed= time - mStartTime ;
			logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER_TUNING);
			logger << "Destination height reached in " << elapsed << " secs" ;
			logger.endMessage() ;
			
			mMode = Mode::IDLE ;
			updateIdle(time, dt, out, gear) ;
			
			mDataDumpMode = true;
			mDataDumpStartTime = time;
		}
		else
		{
			//
			// Get the output from the PID controller
			//
			out = mHeightPID.getOutput(mTarget, height, dt);
			
			//
			// Don't left the motors change voltage to quickly to prevent any
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
			
			logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER_TUNING);
			logger << "lifter mode HEIGHT";
			logger << ", time " << time;
			logger << ", dt "<< dt;
			logger << ", target " << mTarget;
			logger << ", height " << mCurrent ;
			logger << ", ticks " << mTicks ;
			logger << ", out " << out;
			logger.endMessage() ;
		}
	}
}

void LifterController::updateClimb(double time, double dt, double &out, Gear &gear, bool &brake)
{
	paramsInput *p = paramsInput::get() ;
	out = p->getValue("lifter:climb_power", 0.6) ;
	gear = Gear::Low ;
	mCalibrated = false ;
	
	int climb_height_ticks = p->getValue("lifter::climbing_difference", 100) ;
	if (mTicks < mClimbBase - climb_height_ticks)
	{
		//
		// We have reached our climb goal, move to the maintain state
		//
		mMode = Mode::MAINTAIN ;
		break
	}
}

void LifterController::updateMaintain(double time, double dt, double &out, Gear &gear, bool &brake)
{
	paramsInput *p = paramsInput::get() ;
	int climb_height_ticks = p->getValue("lifter:climbing_difference", 100) ;
	int threshold = static_cast<int>(p->getValue("lifter:maintain_climb_threshold", 10)) ;
	
	gear = Gear::LOW ;
	mCalibrated = false ;
	if (mTicks > mClimbBase - climb_height_ticks + threshold)
	{
	    brake = false ;
		out.power = -1.0 ;
	}
	else
	{
	    brake = true ;
		out.power = 0.0 ;
	}
}

void LifterController::updateUp(double time, double dt, double &out, Gear &gear, bool &brake)
{
	paramsInput *p = paramsInput::get() ;
	
	double top_limit = input_params->getValue("lifter:height:top_limit", 96.0);
    double slowdown_range = input_params->getValue("lifter:slowdown_range", 6.0);

	gear = mGear ;
	brake = false ;

	if (mCurrent >= top_limit)
		out = 0.0 ;
	else if (mCurrent > top_limit - slowdown_range)
		out = p->getValue("lifter:slowdown_power", 0.2) ;
	else if (mHighPower)
		out = p->getValue("lifter:manual_power:high", 0.8) ;
	else
		out = p->getValue("lifter:manual_power:low", 0.4) ;
}

void LifterController::updateDown(double time, double dt, double &out, Gear &gear, bool &brake)
{
	paramsInput *p = paramsInput::get() ;
	
    double bottom_limit = input_params->getValue("lifter:collector_offset", 11.375) ;
    double slowdown_range = input_params->getValue("lifter:slowdown_range", 6.0);
	
	gear = mGear ;
	brake = false ;
	if (mCurrent <= bottom_limit)
		out = 0.0 ;
	else if (mCurrent < bottom_limit + slowdown_range)
		out = -p->getValue("lifter:slowdown_power", 0.2) ;
	else if (mHighPower)
		out = -p->getValue("lifter:manual_power:high", 0.8) ;
	else
		out = -p->getValue("lifter:manual_power:low", 0.4) ;
}

void LifterController::updateLocked(double time, double dt, double &out, Gear &gear, bool &brake)
{
	out = 0.0 ;
	gear = mGear ;
	brake = true ;
}

void LifterController::update(double height, int ticks, double time, double dt, double& out, Gear &gear, bool &brake)
{
	//
	// Remember the current ticks number
	//
	mTicks = ticks ;
	
	//
	// Remember the previous height
	//
	mLastHeight = mCurrent ;

	//
	// Calculate the current height based on the ticks
	//
	calcHeight() ;

	//
	// Find the speed of the lift in inches per second
	//
	mSpeed = (mCurrent - mLastHeight) / dt ;

	//
	// Default state of the lifter is do nothing
	//
	out = 0.0 ;
	gear = mGear ;
	brake = false ;

	
	
	switch(mMode)
	{
	case Mode::IDLE:
		updateIdle(time, dt, out, gear, brake) ;
		break ;

	case Mode::HEIGHT:
		updateHeight(time, dt, out, gear, brake) ;
		break ;

	case Mode::CLIMB:
		updateClimb(time, dt, out, gear, brake) ;
		break ;

	case Mode::MAINTAIN:
		updateMaintain(time, dt, out, gear, brake) ;
		break ;

	case Mode::UP:
		updateUp(time, dt, out, gear, brake) ;
		break;

	case Mode::DOWN:
		updateDown(time, dt, out, gear, brake) ;
		break ;
	}
	
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

double LifterController::presetToHeight(Preset preset)
{
	double ret = 11.375 ;
	paramsInput *p = paramsInput::get() ;
	
	
    switch(preset) {
    case Preset::FLOOR:
		ret = p->getValue("lifter:height:floor", 0.0);
		break ;
		
    case Preset::EXCHANGE:
		ret = p->getValue("lifter:height:exchange", 3.0);
		break ;
		
    case Preset::DROP_GRABBER:
		ret = p->getValue("lifter:height:drop_grabber", 6.0);
		break ;
		
    case Preset::SWITCH:
		ret = p->getValue("lifter:height:switch", 19.0);
		break ;
		
    case Preset::SCALE:
		ret = p->getValue("lifter:height:scale", 72.0);
		break ;
		
    case Preset::PREP_CLIMB:
		ret = p->getValue("lifter:height:prep_climb", 84.0);
		break ;
		
    default:
		assert(0);
    }

	return ret ;
}

#ifdef SAVE
    last.upper_slowdown_range = last.height > (top_limit - slowdown_range);
    last.lower_slowdown_range = last.height < (bottom_limit + slowdown_range);

#endif
