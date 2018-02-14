#include "gear_shifter.h"
#include <math.h>

using namespace std;

#define SHIFTER_SOLENOID_LOW 0
#define SHIFTER_SOLENOID_HIGH 1

Robot_outputs Gear_shifter::Output_applicator::operator()(Robot_outputs robot,Output out)const{
	robot.solenoid[SHIFTER_SOLENOID_LOW] = out == Output::LOW;
	robot.solenoid[SHIFTER_SOLENOID_HIGH] = out != Output::LOW;
	return robot;
}

Gear_shifter::Output Gear_shifter::Output_applicator::operator()(Robot_outputs robot)const{
	return robot.solenoid[SHIFTER_SOLENOID_LOW] ? Output::LOW : Output::HIGH;
}

Gear_shifter::Estimator::Estimator():l_tracker(),r_tracker(),last_current(0),last_output(Output::LOW),recommended(Output::LOW),no_shift(),print_count(0){}

Gear_shifter::Status_detail Gear_shifter::Estimator::get()const{
	return recommended;
}

void Gear_shifter::Estimator::update(Time now,Input in,Output out){
	print_count++;
	//cout<<"distance:"<<ticks_to_inches(in.ticks.l)<<" "<<ticks_to_inches(in.ticks.r)<<"\n";
	//if(print_count % 10 == 0) cout<<"Shift Reason:";

	static const double SHIFT_DELAY=2; //seconds between shifts
	no_shift.update(now,1);
	if(out!=last_output) no_shift.set(SHIFT_DELAY);
	last_output=out;
	if(!no_shift.done()){
		recommended=out;
		//if(print_count % 10 == 0) cout<<"no_shift\n";
		return;
	}

	const double CURRENT_SPIKE_THRESHOLD=-8;//TODO: Test this value
	const double CURRENT_SHIFT_SPEED_THRESHOLD = 1.0;//feet/second

	double current_spike=sum(in.current)-last_current;
	last_current=sum(in.current);
	//if(print_count % 10 == 0) cout<<" difference_in_current:"<<current_spike;
	
	l_tracker.update(now,in.distances.l);
	r_tracker.update(now,in.distances.r);
	
	static const double INCHES_TO_FEET = 1.0/12.0;
	
	Drivebase::Speeds speeds = {fabs(l_tracker.get() * INCHES_TO_FEET), fabs(r_tracker.get() * INCHES_TO_FEET)}; //ft/second
	
	//if(print_count % 10 == 0) cout<<" speeds:"<<speeds<<" ";
	
	if(current_spike < CURRENT_SPIKE_THRESHOLD && speeds.l < CURRENT_SHIFT_SPEED_THRESHOLD && speeds.r < CURRENT_SHIFT_SPEED_THRESHOLD){
		recommended=Output::LOW;
		//if(print_count % 10 == 0)cout<<"current_spike\n";
		return;
	}

	const double TURN_THRESHOLD=1.2;
	if(speeds.l>speeds.r*TURN_THRESHOLD || speeds.r>speeds.l*TURN_THRESHOLD){
		recommended=out;
		//if(print_count % 10 == 0) cout<<"turning\n";
		return;
	}

	double mean_speed=mean(speeds.l,speeds.r);
	
	static const double LOW_SPEED_THRESHOLD = 5.0, HIGH_SPEED_THRESHOLD = 5.0;//feet/second

	if(mean_speed<LOW_SPEED_THRESHOLD){
		recommended=Output::LOW;
		//if(print_count % 10 == 0) cout<<"speed low\n";
		return;
	}
	if(mean_speed>HIGH_SPEED_THRESHOLD){
		recommended=Output::HIGH;
		//if(print_count % 10 == 0) cout<<"speed high\n";
		return;
	}

	//if(print_count % 10 == 0) cout<<"none\n";
	recommended=out;
}

CMP_OPS(Gear_shifter::Estimator,GEAR_SHIFTER_ESTIMATOR)

ostream& operator<<(ostream& o,Gear_shifter::Goal const& g){
	#define X(A) if(g==Gear_shifter::Goal::A) return o<<"Gear_shifter::Goal("#A")";
	GEAR_SHIFTER_GOALS
	#undef X
	assert(0); 
}

ostream& operator<<(ostream& o,Gear_shifter::Output const& out){
	#define X(A) if(out==Gear_shifter::Output::A) return o<<"Gear_shifter::Output("#A")";
	GEAR_SHIFTER_OUTPUTS
	#undef X
	assert(0); 
}

ostream& operator<<(ostream& o,Gear_shifter const& a){
	o<<"Gear_shifter(";
	o<<a.estimator.get();
	return o<<")";
}

set<Gear_shifter::Goal> examples(Gear_shifter::Goal*){
	#define X(A) Gear_shifter::Goal::A,
	return {GEAR_SHIFTER_GOALS};
	#undef X
}

set<Gear_shifter::Output> examples(Gear_shifter::Output*){
	#define X(A) Gear_shifter::Output::A,
	return {GEAR_SHIFTER_OUTPUTS};
	#undef X
}

Gear_shifter::Status status(Gear_shifter::Status_detail){
	return {};
}

Gear_shifter::Output control(Gear_shifter::Status_detail status,Gear_shifter::Goal goal){
	switch(goal) {
		case Gear_shifter::Goal::AUTO: return status;
		case Gear_shifter::Goal::LOW: return Gear_shifter::Output::LOW;
		case Gear_shifter::Goal::HIGH: return Gear_shifter::Output::HIGH;
		default: assert(0);
	}
}

bool ready(Gear_shifter::Status,Gear_shifter::Goal){
	return 1;
}

#ifdef GEAR_SHIFTER_TEST
#include "formal.h"
int main(){
	{
		Gear_shifter g;
		tester(g);
	}

	/*
	This got broken at the moment.  Sorry.
	{
		cout<<"\nTEST 1\n";
		Gear_shifter g;
		Gear_shifter::Goal goal = Gear_shifter::Goal::AUTO;

		const Time SPIKE_TIME = 10;
		const double SPIKE_VALUE = -14;
		const int TICKS_INCREMENT = 500;

		Drivebase::Encoder_ticks ticks = {0,0};
		array<double,Drivebase::MOTORS> currents = {0,0,0,0,0,0};
			
		for(Time t = 0; t < 100; t+=0.05){
			Gear_shifter::Status_detail status = g.estimator.get();
			Gear_shifter::Output out = control(status,goal);
			
			if(t >= SPIKE_TIME) currents = {SPIKE_VALUE,SPIKE_VALUE,SPIKE_VALUE,SPIKE_VALUE,SPIKE_VALUE,SPIKE_VALUE};
			else ticks = {ticks.l + TICKS_INCREMENT, ticks.r + TICKS_INCREMENT};
	
			Gear_shifter::Input in = {currents,{},{},ticks};
			
			cout<<"t:"<<t<<"\tin:"<<in<<"\tstatus:"<<status<<"\tgoal:"<<goal<<"\tout:"<<out<<"\n";
			
			g.estimator.update(t,in,out);
			cout<<"\n----------------------------------------------------------------\n";
		}
	}
	*/
}
#endif
