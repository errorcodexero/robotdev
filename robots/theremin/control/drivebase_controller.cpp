#include "drivebase_controller.h"
#include "message_logger.h"
#include "UdpBroadcastSender.h"

#include <cmath>
#include <iostream>

int seq = 0;

DrivebaseController::DrivebaseController() {
    mMode = Mode::IDLE;
    mTarget = 0.0;
    mTargetCorrectionAngle = 0.0;
    mDistanceThreshold = 0.0;
    mAngleThreshold = 0.0;
    mAngleVThreshold = 0.0;
    mLastAngle = 0.0;
    mNsamples = 5;
    mResetPid = false;
    mPidResetThreshold = 0.0;
    mLastVoltage = 0.0;
    mMaxChange = 6.0;
}

void DrivebaseController::setParams(paramsInput* input_params) {
    mInputParams = input_params;
    mDistanceThreshold = mInputParams->getValue("drivebase:straight:threshold", 1.0);
    mAngleThreshold = mInputParams->getValue("drivebase:angle:threshold", 1.0);
    mAngleVThreshold = mInputParams->getValue("drivebase:angle:v_threshold", .1);
    mNsamples = mInputParams->getValue("drivebase:mNsamples", 5);
    mPidResetThreshold = mInputParams->getValue("drivebase:distance:reset:threshold", .1);
}

void DrivebaseController::initDistance(double distance, double angle) {

    mMode = Mode::DISTANCE;
    mTarget = distance;
    mTargetCorrectionAngle = angle;
    mResetPid = false;
    mDistanceHistory.clear();

    double p = mInputParams->getValue("drivebase:distance:p", 0.015);
    double i = mInputParams->getValue("drivebase:distance:i", 0.1);
    double d = mInputParams->getValue("drivebase:distance:d", 0.0);
    double f = mInputParams->getValue("drivebase:distance:f", 0.0);
    double imax = mInputParams->getValue("drivebase:distance:imax", 6.0);

    mDistPid.Init(p, i, d, f, -0.6, 0.6, imax);

    double ap = mInputParams->getValue("drivebase:angleCorrection:p", 0.0);
    double ai = mInputParams->getValue("drivebase:angleCorrection:i", 0.0);
    double ad = mInputParams->getValue("drivebase:angleCorrection:d", 0.0);
    double af = mInputParams->getValue("drivebase:angleCorrection:f", 0.0);
    double aimax = mInputParams->getValue("drivebase:angleCorrection:imax", 0.0);

    mStraightnessPid.Init(ap, ai, ad, af, -0.3, 0.3, aimax, false);

    messageLogger &logger = messageLogger::get();
    logger.startMessage(messageLogger::messageType::debug);
    logger << "initDistance, distance = " << distance;
    logger << ", dpid " << p << " " << i << " " << d << " " << f << " " << imax;
    logger << ", apid " << ap << " " << ai << " " << ad << " " << af << " " << imax;
    logger.endMessage();
}

void DrivebaseController::initAngle(double angle) {
    mMode = Mode::ANGLE;
    mTarget = angle;

    double ap = mInputParams->getValue("drivebase:angle:p", 0.0);
    double ai = mInputParams->getValue("drivebase:angle:i", 0.0);
    double ad = mInputParams->getValue("drivebase:angle:d", 0.0);
    double af = mInputParams->getValue("drivebase:angle:f", 0.0);
    double aimax = mInputParams->getValue("drivebase:angle:imax", 0.0);

    mAnglePid.Init(ap, ai, ad, af, -0.6, 0.6, aimax, false);

    messageLogger &logger = messageLogger::get();
    logger.startMessage(messageLogger::messageType::debug);
    logger << "initAngle, angle = " << angle;
    logger << ", apid " << ap << " " << ai << " " << ad << " " << af << " " << aimax;
    logger.endMessage();
}

void DrivebaseController::idle(double distances_l, double distances_r, double angle, double dt, double time)
{
    if (mDataDumpMode)
    {
	double avg_dist = (distances_l + distances_r) / 2.0;

	messageLogger &logger = messageLogger::get();
	logger.startMessage(messageLogger::messageType::debug);
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
				 double& out_l, double& out_r)
{
    messageLogger &logger = messageLogger::get();

    if (mMode == Mode::DISTANCE) {
	double avg_dist = (distances_l + distances_r) / 2.0;
	if ((mTarget - avg_dist) < mDistanceThreshold)
	    mMode = Mode::IDLE;

	mDistanceHistory.push_back(avg_dist);
	if (mDistanceHistory.size() > mNsamples)
	    mDistanceHistory.pop_front();

	logger.startMessage(messageLogger::messageType::debug);
	if (!mResetPid && mDistanceHistory.size() == mNsamples && (mDistanceHistory.back() - mDistanceHistory.front()) < mPidResetThreshold) {
	    logger << "SWITCHED PID CONSTANTS\n";

	    double p = mInputParams->getValue("drivebase:distance:reset:p", 0.0);
	    double i = mInputParams->getValue("drivebase:distance:reset:i", 0.15);
	    double d = mInputParams->getValue("drivebase:distance:reset:d", 0.0);
	    double f = mInputParams->getValue("drivebase:distance:reset:f", 0.0);
	    double imax = mInputParams->getValue("drivebase:distance:reset:imax", 10.0);
	    mDistPid.Init(p, i, d, f, -0.6, 0.6, imax);

	    mResetPid = true;
	}

	double base = mDistPid.getOutput(mTarget, avg_dist, dt);

#ifdef NOTYET
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
	if (mMode == Mode::IDLE)
	    logger << ", REACHED";
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
	logger.startMessage(messageLogger::messageType::debug);
	logger << "update(ANGLE)";
	logger << ", time " << time;
	logger << ", target " << mTarget;
	logger << ", angle " << angle;
	logger << ", angular_v " << angular_v;
	logger << ", base " << base;
	logger << ", l " << out_l;
	logger << ", r " << out_r;
	if (mMode == Mode::IDLE)
	    logger << ", REACHED";
	logger.endMessage();
    }

    if (mMode == Mode::IDLE) {
	out_l = 0.0;
	out_r = 0.0;
	mDataDumpMode = true;
	mDataDumpStartTime = time;
    }

    mLastAngle = angle;
}

bool DrivebaseController::done() {
    return mMode == Mode::IDLE;
}
