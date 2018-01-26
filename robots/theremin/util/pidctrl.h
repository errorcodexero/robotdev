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
	PIDCtrl();
	PIDCtrl(double p, double i, double d, double f, double floor, double ceil, double integralCeil);
	void Init(double p, double i, double d, double f, double floor, double ceil, double integralCeil);
	double getOutput(double target, double current, double timeDifference);
};
