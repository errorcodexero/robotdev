#ifndef DRIVEBASE_CONTROLLER_H
#define DRIVEBASE_CONTROLLER_H

#include "pidctrl.h"
#include "params_parser.h"
#include "UdpBroadcastSender.h"

class DrivebaseController {
public:

	DrivebaseController();

	void setParams(paramsInput*);

	void initDistance(double);
	void initAngle(double);

	void update(double dl, double dr, double angle, double dt, double time,
				double &out_l, double &out_r, bool &out_zero_yaw);

	void idle(double dl, double dr, double angle, double dt, double time);

	bool done();
	private:
	enum class Mode {
		IDLE,
		DISTANCE,
		ANGLE
	};
	Mode mode;

	bool zero_yaw;

	double target;
	double distance_threshold, angle_threshold;
	PIDCtrl straightness_pid, dist_pid, angle_pid;
	paramsInput* mInput_params;

	double mLastVoltage ;
	double mMaxChange ;

	double mLeftStart ;
	double mRightStart ;

	bwgnet::UdpBroadcastSender mSender ;

	bool mDataDumpMode ;
	double mDataDumpStartTime ;

};

#endif
