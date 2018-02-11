#ifndef LIFTER_CONTROLLER_H
#define LIFTER_CONTROLLER_H

#include "pidctrl.h"
#include "params_parser.h"

class LifterController {
public:
	enum class Preset {
		FLOOR,
		EXCHANGE,
		SWITCH,
		SCALE,
		PREP_CLIMB
	};

	LifterController();

	void setParams(paramsInput* input_params);
	void initHeight(double height);
	void initHeight(Preset preset);
	void update(double height, double time, double dt, double &out);
	void idle(double height, double time, double dt);
	void initOnChange(double height);
	void initOnChange(Preset preset);
	bool done();

	double presetToHeight(Preset preset);

private:
	enum class Mode {
		IDLE,
		HEIGHT
	};
	Mode mMode;
	
	double mTarget;
	double mLastTarget;
	double mHeightThreshold;

	PIDCtrl mHeightPID;

	paramsInput* mInputParams;

	double mLastVoltage;
	double mMaxChange;

	bool mDataDumpMode;
	double mDataDumpStartTime;
};

#endif
