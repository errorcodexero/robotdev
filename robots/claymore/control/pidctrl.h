#pragma once

class PIDCtrl {
private:
	struct PIDConstants {
		double p, i, d, f;
		double floor, ceil;
		double integralCeil;
	} PIDConsts;
	double target, current, timeDifference;
	double integral;
	
public:
	PIDCtrl(double p, double i, double d, double f, double floor, double ceil);
	void Init(double p, double i, double d, double f, double floor, double ceil);
	double getOutput(double target, double current, double timeDifference);
};