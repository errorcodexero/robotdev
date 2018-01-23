#include "intake.h"
#include <stdlib.h>
#include <cmath>

using namespace std;

#define INTAKE_ADDRESS_R 2
#define INTAKE_ADDRESS_L 3
#define INTAKE_POWER 1.0 //TODO tune this

ostream& operator<<(ostream& o, Intake::Goal a){
	#define X(name) if(a==Intake::Goal::name)return o<<"Intake::Goal("#name")";
	X(IN) X(OUT) X(OFF)
	#undef X
	assert(0);
}

ostream& operator<<(ostream& o, Intake){ return o<<"Intake()";}

bool operator==(Intake::Output_applicator, Intake::Output_applicator){return 1;}

bool operator==(Intake a, Intake b){ return (a.input_reader==b.input_reader && a.estimator==b.estimator && a.output_applicator==b.output_applicator);}
bool operator!=(Intake a, Intake b){ return !(a==b);}

Robot_outputs Intake::Output_applicator::operator()(Robot_outputs r, Intake::Output out)const{
	double out_power;
	switch(out) {
		case Intake::Output::OUT:
			out_power = -INTAKE_POWER;
			break;
		case Intake::Output::OFF:
			out_power = 0.0;	
			break;
		case Intake::Output::IN:
			out_power = INTAKE_POWER;
			break;
		default:
			assert(0);
	}
	r.pwm[INTAKE_ADDRESS_R] = out_power;
	r.pwm[INTAKE_ADDRESS_L] = -out_power;
	return r;
}

Intake::Output Intake::Output_applicator::operator()(Robot_outputs r)const{
	//Assuming left and right motors are doing the same thing
	double pwm = r.pwm[INTAKE_ADDRESS_R];
	if(pwm < 0.0) return Intake::Output::OUT;
	if(pwm == 0.0) return Intake::Output::OFF;
	if(pwm > 0.0) return Intake::Output::IN;
	assert(0);
}
	
set<Intake::Goal> examples(Intake::Goal*){ 
	return {Intake::Goal::OUT, Intake::Goal::OFF, Intake::Goal::IN};
}

Intake::Output control(Intake::Status_detail, Intake::Goal goal){
	return goal;
}

bool ready(Intake::Status, Intake::Goal){ return 1;}

#ifdef INTAKE_TEST
#include "formal.h"

int main(){
	Intake a;
	tester(a);
}

#endif
