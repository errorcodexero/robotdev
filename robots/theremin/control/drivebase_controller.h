#ifndef DRIVEBASE_CONTROLLER_H
#define DRIVEBASE_CONTROLLER_H

#include <list>
#include "pidctrl.h"
#include "params_parser.h"
#include "UdpBroadcastSender.h"

class DrivebaseController {
public:

	DrivebaseController();

	void setParams(paramsInput*);

	void initDistance(double, double);
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

	double target, target_correction_angle;
	double distance_threshold, angle_threshold, angle_v_threshold;
	PIDCtrl straightness_pid, dist_pid, angle_pid;
	
	double last_angle;

	std::list<double> distance_history;
	size_t n_samples;
	bool reset_pid;
	double pid_reset_threshold;

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
