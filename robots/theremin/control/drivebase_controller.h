#ifndef DRIVEBASE_CONTROLLER_H
#define DRIVEBASE_CONTROLLER_H

#include "../util/pidctrl.h"

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

	public:
	DrivebaseController();

	void initDistance(double);
	void initAngle(double);

	void update(double, double, double, double, double&, double&, bool&);
	bool done();
};

#endif
