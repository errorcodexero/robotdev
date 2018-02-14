#include "wings.h"
#include <stdlib.h>
#include <cmath>

using namespace std;

#define WINGS_ADDRESS 4

ostream& operator<<(ostream& o, Wings::Goal a){
	#define X(name) if(a==Wings::Goal::name)return o<<"Wings::Goal("#name")";
	X(LOCKED) X(UNLOCKED)
	#undef X
	assert(0);
}

ostream& operator<<(ostream& o, Wings){ return o<<"Wings()";}

bool operator==(Wings::Output_applicator, Wings::Output_applicator){return 1;}

bool operator==(Wings a, Wings b){ return (a.input_reader==b.input_reader && a.estimator==b.estimator && a.output_applicator==b.output_applicator);}
bool operator!=(Wings a, Wings b){ return !(a==b);}

Robot_outputs Wings::Output_applicator::operator()(Robot_outputs r, Wings::Output out)const{
	r.solenoid[WINGS_ADDRESS] = out == Output::LOCKED;
	return r;
}

Wings::Output Wings::Output_applicator::operator()(Robot_outputs r)const{
	return r.solenoid[WINGS_ADDRESS] ? Output::LOCKED : Output::UNLOCKED;
}
	
set<Wings::Goal> examples(Wings::Goal*){ 
	return {Wings::Goal::LOCKED, Wings::Goal::UNLOCKED};
}

Wings::Output control(Wings::Status_detail, Wings::Goal goal){
	return goal;
}

bool ready(Wings::Status, Wings::Goal){ return 1;}

#ifdef WINGS_TEST
#include "formal.h"

int main(){
	Wings a;
	tester(a);
}

#endif
