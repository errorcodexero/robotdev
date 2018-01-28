#include "lights.h"
#include <stdlib.h>
#include <cmath>

using namespace std;

#define CAMERA_LIGHT_ADDRESS 4
#define BLINKY_LIGHT_INFO_ADDRESS 5

Lights::Goal::Goal(Camera_light a):camera_light(a){}
Lights::Goal::Goal():Lights::Goal(Camera_light::OFF){}

Lights::Output::Output(bool c,double b):camera_light(c),blinky_light_info(b){}
Lights::Output::Output():Output(false,0.0){}

Lights::Status_detail::Status_detail(unsigned h,bool c,bool m,Alliance a,Time t):lifter_height(h),climbing(c),autonomous(m),alliance(a),now(t){}
Lights::Status_detail::Status_detail():Status_detail(0.0,false,false,Alliance::INVALID,0){}

ostream& operator<<(ostream& o, Lights::Camera_light a){
	#define X(name) if(a==Lights::Camera_light::name)return o<<""#name;
	X(OFF) X(ON)
	#undef X
	nyi
}

ostream& operator<<(ostream& o, Lights::Status_detail a){
	o<<"(";
	o<<" lifter_height:"<<a.lifter_height;
	o<<" climbing:"<<a.climbing;
	o<<" autonomous:"<<a.autonomous;
	o<<" alliance:"<<a.alliance;
	o<<" now:"<<a.now;
	o<<")";
	return o;
}

ostream& operator<<(ostream& o, Lights::Output a){
	o<<"(";
	o<<"camera_light:"<<a.camera_light;
	o<<"blinky_light_info:"<<a.blinky_light_info;
	o<<")";
	return o;
}

ostream& operator<<(ostream& o, Lights::Goal a){
	o<<"(";
	o<<" camera_light:"<<a.camera_light;
	return o<<")";
}

ostream& operator<<(ostream& o, Lights){ return o<<"Lights()";}

#define CMP(ELEM) \
	if(a.ELEM < b.ELEM) return true; \
	if(b.ELEM < a.ELEM) return false;

bool operator==(Lights::Status_detail a,Lights::Status_detail b){
	return a.lifter_height == b.lifter_height && a.climbing == b.climbing && a.autonomous == b.autonomous && a.alliance == b.alliance && a.now == b.now;
}

bool operator!=(Lights::Status_detail a,Lights::Status_detail b){
	return !(a==b);
}

bool operator<(Lights::Status_detail a,Lights::Status_detail b){
	CMP(lifter_height)
	CMP(climbing)
	CMP(autonomous)
	CMP(alliance)
	CMP(now)
	return false;
}

bool operator==(Lights::Output a,Lights::Output b){
	return a.camera_light == b.camera_light && a.blinky_light_info == b.blinky_light_info;
}

bool operator!=(Lights::Output a,Lights::Output b){
	return !(a==b);
}

bool operator<(Lights::Output a,Lights::Output b){
	CMP(camera_light)
	CMP(blinky_light_info)
	return false;
}

bool operator==(Lights::Goal a,Lights::Goal b){
	return a.camera_light == b.camera_light && a.blinky_light_info == b.blinky_light_info;
}

bool operator<(Lights::Goal a,Lights::Goal b){
	if(a.camera_light<b.camera_light) return 1;
	if(b.camera_light<a.camera_light) return 0;
	return 0;
}

bool operator!=(Lights::Goal a, Lights::Goal b){
	return !(a==b);
}

bool operator==(Lights::Output_applicator, Lights::Output_applicator){
	return 1;
}

bool operator==(Lights::Estimator, Lights::Estimator){
	return true;
}

bool operator!=(Lights::Estimator a, Lights::Estimator b){
	return !(a==b);
}

bool operator==(Lights a, Lights b){
	return (a.input_reader==b.input_reader && a.estimator==b.estimator && a.output_applicator==b.output_applicator);
}

bool operator!=(Lights a, Lights b){
	return !(a==b);
}

#undef CMP

Lights::Output Lights::Output_applicator::operator()(Robot_outputs r)const{
	Output out;
	out.camera_light = r.digital_io[CAMERA_LIGHT_ADDRESS] == Digital_out::one();
	out.blinky_light_info = r.pwm[BLINKY_LIGHT_INFO_ADDRESS];
	return out;
}

Robot_outputs Lights::Output_applicator::operator()(Robot_outputs r, Lights::Output out)const{
	r.digital_io[CAMERA_LIGHT_ADDRESS] = out.camera_light ? Digital_out::one() : Digital_out::zero();
	r.pwm[BLINKY_LIGHT_INFO_ADDRESS] = out.blinky_light_info;
	return r;
}

Lights::Estimator::Estimator(){}

void Lights::Estimator::update(Time,Lights::Input,Lights::Output){}

Lights::Status_detail Lights::Estimator::get()const{
	return {};
}

set<Lights::Status_detail> examples(Lights::Status_detail*){ 
	set<Lights::Status_detail> s;
	vector<bool> bools = {false,true};
	vector<Alliance> alliances = {Alliance::RED,Alliance::BLUE,Alliance::INVALID};
	for(bool c: bools){
		for(bool m: bools){
			for(Alliance a: alliances){
				s.insert({0,c,m,a,0.0});
			}
		}
	}
	return s;
}

set<Lights::Output> examples(Lights::Output*){ 
	return {
		Lights::Output{false,0.0},
		Lights::Output{true,0.0}	
	};
}

set<Lights::Goal> examples(Lights::Goal*){ 
	return {
		Lights::Goal{Lights::Camera_light::ON},
		Lights::Goal{Lights::Camera_light::OFF}
	};
}

double encode_robot_status(Lights::Status_detail status){
	const unsigned INFO_TYPES = 4;//number of types of data for transmission
	
	//values increase by one as we go
	const map<bool, int> ENCODE_CLIMBING = {
		{false, 0}, //not climbing 
		{true, 1} //climbing
	};
	const map<bool, int> ENCODE_AUTONOMOUS = {
		{false, 2}, //teleop 
		{true, 3} //autonomous
	};
	const map<Alliance, int> ENCODE_ALLIANCE = {
		{Alliance::INVALID, 4},
		{Alliance::RED, 5},
		{Alliance::BLUE, 6} 
	};
	int encode_lifter_height = status.lifter_height + 7;
	//

	double time_remainder = [&]{
		double remainder, integer;
		remainder = modf(status.now, &integer);
		return remainder;
	}();
	
	const Time TRANSMIT_TIME = 0.1;//seconds
	const Time TIME_INTERVAL = TRANSMIT_TIME / INFO_TYPES;//seconds to transmit each part of data

	int to_transmit = [&]{
		for(unsigned i = 1; i <= INFO_TYPES; i++){
			if(time_remainder < TIME_INTERVAL * i){
				to_transmit = i;
			}
		}
		return to_transmit;
	}();
	
	int transmit_value = [&]{
		switch(to_transmit){
			case 1:
				return ENCODE_CLIMBING.at(status.climbing);
			case 2:
				return ENCODE_AUTONOMOUS.at(status.autonomous);
			case 3:
				return ENCODE_ALLIANCE.at(status.alliance);
			case 4: 
				return encode_lifter_height;
			default:
				nyi
		}
	}();

	const unsigned INFO_STATES = 107;//number of possible values to send (lifter height may be clipped at 100 in, but that's fine)
	const double PWM_RANGE = 2.0;
	const double INCREMENT = PWM_RANGE / (double)INFO_STATES;
	const double STARTING_VALUE = -1.0;
	
	return STARTING_VALUE + INCREMENT * transmit_value; 
}

Lights::Output control(Lights::Status status, Lights::Goal goal){
	Lights::Output out;
	out.camera_light = goal.camera_light == Lights::Camera_light::ON;
	out.blinky_light_info = encode_robot_status(status);
	return out;
}

bool ready(Lights::Status, Lights::Goal){ return 1;}

Lights::Status status(Lights::Status_detail const& a){
	return a;
}

#ifdef LIGHTS_TEST
#include "formal.h"

int main(){
	Lights a;
	Tester_mode mode;
	mode.check_outputs_exhaustive = false;
	tester(a,mode);
}

#endif
