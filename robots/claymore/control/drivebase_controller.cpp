#include "drivebase_controller.h"
#include "message_logger.h"
#include "UdpBroadcastSender.h"
#include "subsystems.h"

#include <SmartDashboard/SmartDashboard.h>
#include <cmath>
#include <iostream>

#ifdef TUNING
std::string DrivebaseController::AngleTargetName("angle-target") ;
std::string DrivebaseController::AngleActualName("angle-actual") ;
std::string DrivebaseController::AngleVelocityName("angle-velocity") ;
std::string DrivebaseController::AnglePName("angle-p") ;
std::string DrivebaseController::AngleIName("angle-i") ;
std::string DrivebaseController::AngleDName("angle-d") ;
std::string DrivebaseController::AngleTimeName("angle-time") ;
#endif

DrivebaseController::DrivebaseController() {
    mMode = Mode::IDLE;
    mTarget = 0.0;
    mTargetCorrectionAngle = 0.0;
    mDistanceThreshold = 0.0;
    mAngleThreshold = 0.0;
    mAngleVThreshold = 0.0;
    mLastDistance = 0.0;
    mLastAngle = 0.0;
    mNsamples = 5;
    mResetPid = false;
    mPidResetThreshold = 0.0;
    mLastVoltage = 0.0;
    mMaxChange = 6.0;
    mCycleInterval = 4 ;
    mCurrentCycle = 0 ;
    mLastLeftVoltage = 0.0 ;
    mLastRightVoltage = 0.0 ;
    mHighGear = false;

#ifdef TUNING
    frc::SmartDashboard::SetPersistent(AngleTargetName) ;
    frc::SmartDashboard::SetPersistent(AngleActualName) ;
    frc::SmartDashboard::SetPersistent(AngleVelocityName) ;
    frc::SmartDashboard::SetPersistent(AnglePName) ;
    frc::SmartDashboard::SetPersistent(AngleIName) ;
    frc::SmartDashboard::SetPersistent(AngleDName) ;

	SmartDashboard::PutNumber(AnglePName, 0.0) ;
	SmartDashboard::PutNumber(AngleIName, 0.0) ;
	SmartDashboard::PutNumber(AngleDName, 0.0) ;
#endif
}

void DrivebaseController::setParams(paramsInput* input_params) {
    mInputParams = input_params;
    mDistanceThreshold = mInputParams->getValue("drivebase:straight:threshold", 1.0);
    mAngleThreshold = mInputParams->getValue("drivebase:angle:threshold", 1.0);
    mAngleVThreshold = mInputParams->getValue("drivebase:angle:v_threshold", .1);
    mNsamples = mInputParams->getValue("drivebase:samples", 5);
    mPidResetThreshold = mInputParams->getValue("drivebase:reset_threshold", .1);
}

paramsInput* DrivebaseController::getParams() {
    return mInputParams;
}

void DrivebaseController::initDistance(double distance, double angle, double time, bool end_on_stall) {

    mMode = Mode::DISTANCE;
    mTarget = distance;
    mTargetCorrectionAngle = angle;
    mResetPid = false;
    mDistanceHistory.clear();
    mTargetStartTime = time ;

    mCurrentCycle = 0 ;
    mLastLeftVoltage = 0.0 ;
    mLastRightVoltage = 0.0 ;

    mEndOnStall = end_on_stall;

    mHighGear = false;

    double p = mInputParams->getValue("drivebase:distance:p", 0.015);
    double i = mInputParams->getValue("drivebase:distance:i", 0.1);
    double d = mInputParams->getValue("drivebase:distance:d", 0.0);
    double f = mInputParams->getValue("drivebase:distance:f", 0.0);
    double imax = mInputParams->getValue("drivebase:distance:imax", 6.0);
	double vmin = mInputParams->getValue("drivebase:distance:vmin", -0.6);
	double vmax = mInputParams->getValue("drivebase:distance:vmax", -0.6);

    mDistPid.Init(p, i, d, f, vmin, vmax, imax);

    double ap = mInputParams->getValue("drivebase:angleCorrection:p", 0.0);
    double ai = mInputParams->getValue("drivebase:angleCorrection:i", 0.0);
    double ad = mInputParams->getValue("drivebase:angleCorrection:d", 0.0);
    double af = mInputParams->getValue("drivebase:angleCorrection:f", 0.0);
    double aimax = mInputParams->getValue("drivebase:angleCorrection:imax", 0.0);

    mStraightnessPid.Init(ap, ai, ad, af, -0.3, 0.3, aimax, false);

    messageLogger &logger = messageLogger::get();
    logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_DRIVEBASE);
    logger << "initDistance, distance = " << distance;
    logger << ", dpid " << p << " " << i << " " << d << " " << f << " " << imax << " " << vmin << " " << vmax ;
    logger << ", apid " << ap << " " << ai << " " << ad << " " << af << " " << imax;
    logger.endMessage();
}

void DrivebaseController::initAngle(double angle, double time) {
    mMode = Mode::ANGLE;
    mTarget = angle;
    mTargetStartTime = time ;
	
    mCurrentCycle = 0 ;
    mLastLeftVoltage = 0.0 ;
    mLastRightVoltage = 0.0 ;

    double ap = mInputParams->getValue("drivebase:angle:p", 0.0);
    double ai = mInputParams->getValue("drivebase:angle:i", 0.0);
    double ad = mInputParams->getValue("drivebase:angle:d", 0.0);
    double af = mInputParams->getValue("drivebase:angle:f", 0.0);
    double aimax = mInputParams->getValue("drivebase:angle:imax", 0.0);
    double minvolts = mInputParams->getValue("drivebase:angle:minv", -0.3) ;
    double maxvolts = mInputParams->getValue("drivebase:angle:maxv", 0.3) ;
    mAnglePid.Init(ap, ai, ad, af, minvolts, maxvolts, aimax, false);

#ifdef TUNING
	ap = SmartDashboard::GetNumber(AnglePName, ap) ;
	ai = SmartDashboard::GetNumber(AngleIName, ai) ;
	ad = SmartDashboard::GetNumber(AngleDName, ad) ;
#endif

    messageLogger &logger = messageLogger::get();
    logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_DRIVEBASE);
    logger << "initAngle, angle = " << angle;
    logger << ", apid " << ap << " " << ai << " " << ad << " " << af << " " << aimax;
    logger << ", minv " << minvolts << ", maxv " << maxvolts ;
    logger.endMessage();
}

void DrivebaseController::idle(double distances_l, double distances_r, double angle, double dt, double time)
{
    if (mDataDumpMode)
    {
		double avg_dist = (distances_l + distances_r) / 2.0;

		messageLogger &logger = messageLogger::get();
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_DRIVEBASE);
		logger << "IDLE: dt " << dt;
		logger << ", time " << time;
		logger << ", angle " << angle;
		logger << ", distance " << avg_dist;
		logger.endMessage();

		if (time - mDataDumpStartTime > 5.0)
			mDataDumpMode = false;
    }
}

void DrivebaseController::update(double distances_l, double distances_r, double angle, double dt, double time,
				 double& out_l, double& out_r, bool& out_high_gear)
{
    messageLogger &logger = messageLogger::get();


    if ((mCurrentCycle % mCycleInterval) == 0)
    {
	double avg_dist = (distances_l + distances_r) / 2.0;
	if (mMode == Mode::DISTANCE) {
	    if ((mTarget - avg_dist) < mDistanceThreshold)
		mMode = Mode::IDLE;

	    mDistanceHistory.push_back(avg_dist);
	    if (mDistanceHistory.size() > mNsamples)
		mDistanceHistory.pop_front();

	    logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_DRIVEBASE);
	    if (mDistanceHistory.size() == mNsamples && (mDistanceHistory.back() - mDistanceHistory.front()) < mPidResetThreshold) {
		if(!mResetPid) {
		    logger << "SWITCHED PID CONSTANTS\n";

		    double p = mInputParams->getValue("drivebase:distance:reset:p", 0.0);
		    double i = mInputParams->getValue("drivebase:distance:reset:i", 0.15);
		    double d = mInputParams->getValue("drivebase:distance:reset:d", 0.0);
		    double f = mInputParams->getValue("drivebase:distance:reset:f", 0.0);
		    double imax = mInputParams->getValue("drivebase:distance:reset:imax", 10.0);
		    mDistPid.Init(p, i, d, f, -0.6, 0.6, imax);

		    mResetPid = true;
		    mDistanceHistory.clear();
		} else {
		    logger << "STALLED\n";

		    mStalled = true;
		}
	    }

	    double base = mDistPid.getOutput(mTarget, avg_dist, dt);

#ifdef LIMIT_VOLTAGE_CHANGE
			double chg = std::fabs(base - mLastVoltage);
			if (chg > mMaxChange * dt)
			{
				if (base > mLastVoltage)
					base = mLastVoltage + mMaxChange * dt;
				else
					base = mLastVoltage - mMaxChange * dt;
			}
#endif

	    mLastVoltage = base;
	    double offset = mStraightnessPid.getOutput(mTargetCorrectionAngle, angle, dt);
	    out_l = base - offset;
	    out_r = base + offset;

	    double low_gear_threshold = mInputParams->getValue("drivebase:distance:low_gear_threshold", 36.0);
	    double high_gear_threshold = mInputParams->getValue("drivebase:distance:high_gear_threshold", 120.0);
	    double velocity_threshold = mInputParams->getValue("drivebase:distance:shift_v_threshold", 0.01);

	    double remaining_distance = fabs(mTarget - avg_dist);
	    double velocity = (avg_dist - mLastDistance) / dt;
	    if(fabs(velocity) > velocity_threshold) {
		if(remaining_distance < low_gear_threshold) { 
		    if(mHighGear) logger << "SHIFTED INTO LOW GEAR\n";
		    mHighGear = false;
		}
		if(remaining_distance > high_gear_threshold) {
		    if(!mHighGear) logger << "SHIFTED INTO HIGH GEAR\n";
		    mHighGear = true;
		}
	    }
	    out_high_gear = mHighGear;

	    logger << "update(DISTANCE)";
	    logger << ", dt " << dt;
	    logger << ", angle " << angle;
	    logger << ", target " << mTarget;
	    logger << ", distance " << avg_dist;
	    logger << ", ldist " << distances_l;
	    logger << ", rdist " << distances_r;
	    logger << ", base " << base;
	    logger << ", offset " << offset;
	    logger << ", l " << out_l;
	    logger << ", r " << out_r;
	    logger << ", velocity " << velocity;
	    logger << ", high_gear " << out_high_gear;
	    if (mMode == Mode::IDLE) {
		logger << ", Success " ;
		logger << (time - mTargetStartTime) ;
		logger << " seconds" ;
	    }
	    logger.endMessage();


	}
	else if (mMode == Mode::ANGLE) {
	    double angular_v = (angle - mLastAngle) / dt;
	    if (fabs(mTarget - angle) < mAngleThreshold && fabs(angular_v) < mAngleVThreshold) {
		mMode = Mode::IDLE;
	    }

	    double base = mAnglePid.getOutput(mTarget, angle, dt);
	    out_l = base;
	    out_r = -base;

	    messageLogger &logger = messageLogger::get();
	    logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_DRIVEBASE);
	    logger << "update(ANGLE)";
	    logger << ", time " << time;
	    logger << ", target " << mTarget;
	    logger << ", angle " << angle;
	    logger << ", angular_v " << angular_v;
	    logger << ", base " << base;
	    logger << ", l " << out_l;
	    logger << ", r " << out_r;
	    
			if (mMode == Mode::IDLE) {
				logger << ", Success " ;
				logger << (time - mTargetStartTime) ;
				logger << " seconds" ;
			}
			logger.endMessage();
		
#ifdef TUNING
			frc::SmartDashboard::PutNumber(AngleTargetName, mTarget) ;
			frc::SmartDashboard::PutNumber(AngleActualName, angle) ;
			frc::SmartDashboard::PutNumber(AngleVelocityName, angular_v) ;
#endif
		}
	
	if (mMode == Mode::IDLE) {
	    out_l = 0.0;
	    out_r = 0.0;
	    mDataDumpMode = true;
	    mDataDumpStartTime = time;
	}
	
	mLastDistance = avg_dist;
	mLastAngle = angle;
	mLastLeftVoltage = out_l ;
	mLastRightVoltage = out_r ;
    }
    else
    {
		out_l = mLastLeftVoltage ;
		out_r = mLastRightVoltage ;
    }
}

bool DrivebaseController::done()
{
    return mMode == Mode::IDLE || (mMode == Mode::DISTANCE && mEndOnStall && mStalled);
}
