#ifndef DRIVEBASE_CONTROLLER_H
#define DRIVEBASE_CONTROLLER_H

#include "../util/pidctrl.h"
#include "params_parser.h"

class DrivebaseController {
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

	public:
	DrivebaseController();

	void setParams(paramsInput*);

	void initDistance(double);
	void initAngle(double);

	void update(double, double, double, double, double, double&, double&, bool&);
	bool done();
};

#endif
