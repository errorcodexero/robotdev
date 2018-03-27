#include "drive.h"
#include "drivecollect.h"
#include <memory>

//
// Drive: Drive straight a specified distance
//

Drive::Drive(Inch dist, bool end_on_stall)
{
	mTargetDistance = dist ;
	mEndOnStall = end_on_stall ;
	mCurve = false;
	mReturnFromCollect = false;
	mInited = false ;
}

Drive::Drive(Inch dist, double curve_start, double angle_offset, bool end_on_stall)
{
	mTargetDistance = dist ;
	mTargetAngleOffset = angle_offset;
	mCurve = true;
	mCurveStart = curve_start ;
	mEndOnStall = end_on_stall;
	mInited = false;
	mReturnFromCollect = false;
}

Drive::Drive(const char *param_p, Inch dist, bool end_on_stall)
{
	mParamName = param_p ;
	mTargetDistance = dist ;
	mEndOnStall = end_on_stall ;
	mCurve = false;
	mReturnFromCollect = false;
	mInited = false ;
}

Drive::Drive(const std::string &param, Inch dist, bool end_on_stall)
{
	mParamName = param ;
	mTargetDistance = dist ;
	mEndOnStall = end_on_stall ;
	mCurve = false;
	mReturnFromCollect = false;
	mInited = false ;
}

Drive::Drive(bool, Inch return_offset)
{
	mReturnOffset = return_offset;
	mCurve = false;
	mReturnFromCollect = true;
	mInited = false;
}

Step::Status Drive::done(Next_mode_info info)
{
    Step::Status ret =  ready(info.status.drive, Drivebase::Goal::drive_straight()) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
    if (ret == Step::Status::FINISHED_SUCCESS)
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Drive step complete" ;
		logger.endMessage() ;
    }
    return ret ;
}

Toplevel::Goal Drive::run(Run_info info)
{
    return run(info, {});
}

Toplevel::Goal Drive::run(Run_info info, Toplevel::Goal goals)
{
    if(!mInited)
	{
		if (mParamName.length() > 0)
		{
			//
			// Based on a parameter name, go get the value from the
			// parameter file
			//
			paramsInput *params_p = paramsInput::get() ;
			mTargetDistance = params_p->getValue(mParamName, mTargetDistance) ;
		} else if (mReturnFromCollect) {
			mTargetDistance = -(Drive_and_collect::distance_travelled + mReturnOffset);
		}
		
		double avg_status = (info.status.drive.distances.l + info.status.drive.distances.r) / 2.0;
		if(!mCurve) {
			Drivebase::drivebase_controller.initDistance(avg_status + mTargetDistance, info.status.drive.angle,
													 info.in.now, mEndOnStall, mTargetDistance >= 0.0);
		} else {
			Drivebase::drivebase_controller.initCurve(avg_status, avg_status + mTargetDistance, mCurveStart, info.status.drive.angle,
													  mTargetAngleOffset, info.in.now, mEndOnStall, mTargetDistance >= 0.0);
		}
		mInited = true ;
    }
    goals.drive = Drivebase::Goal::drive_straight();
    return goals;
}

std::unique_ptr<Step_impl> Drive::clone()const
{
    return std::unique_ptr<Step_impl>(new Drive(*this));
}

bool Drive::operator==(Drive const& a)const
{
    return mTargetDistance == a.mTargetDistance && mEndOnStall == a.mEndOnStall && mInited == a.mInited ;
}

