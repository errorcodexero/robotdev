#include "pidctrl.h"
#include <iostream>

PIDCtrl::PIDCtrl():PIDCtrl(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0)
{
}

PIDCtrl::PIDCtrl(double p, double i, double d, double f, double floor, double ceil, double integralCeil)
{
	Init(p, i, d, f, floor, ceil, integralCeil);
	current = 0;
}

void PIDCtrl::Init(double p, double i, double d, double f, double floor, double ceil, double integralCeil)
{
	PIDConsts.p = p;
	PIDConsts.i = i;
	PIDConsts.d = d;
	PIDConsts.f = f;
	PIDConsts.floor = floor;
	PIDConsts.ceil = ceil;
	PIDConsts.integralCeil = integralCeil;
	integral = 0;
}

double PIDCtrl::getOutput(double target, double current, double timeDifference)
{
	double error = target - current;
	double pOut = PIDConsts.p*error;
	double derivative = (current - this->current) / timeDifference;
	double dOut = PIDConsts.d*derivative;
	
	integral += error*timeDifference;
	
	if (integral > PIDConsts.integralCeil)
		integral = PIDConsts.integralCeil;
	else if (integral < -PIDConsts.integralCeil)
		integral = -PIDConsts.integralCeil;
	
	double iOut = PIDConsts.i * integral;
	
	double output = pOut + iOut + dOut;
	
	std::cout << "integral " << integral ;
	std::cout << ", pOut " << pOut ;
	std::cout << ", iOut " << iOut ;
	std::cout << ", dOut " << dOut ;
	std::cout << std::endl ;

	
	if (output <= PIDConsts.floor)
		output = PIDConsts.floor;
	
	if (output >= PIDConsts.ceil)
		output = PIDConsts.ceil;
	
	return output;
}
