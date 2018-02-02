#include "lights.h"
#include <stdlib.h>
#include <cmath>

using namespace std;

#define CAMERA_LIGHT_ADDRESS 4
#define BLINKY_LIGHT_INFO_ADDRESS 5

PWM_transcriber Lights::blinky_light_transcriber;

void Lights::init_blinky_light_transcriber(){
	blinky_light_transcriber.add("climbing",set<bool>{true,false});
	blinky_light_transcriber.add("autonomous",set<bool>{true,false});
	blinky_light_transcriber.add("alliance",set<Alliance>{Alliance::RED,Alliance::BLUE,Alliance::INVALID});
	set<unsigned> heights;
	for(unsigned i = 0; i < 100; i +=3){
		heights.insert(i);
	}
	blinky_light_transcriber.add("lifter_height",heights);
}

Lights::Goal::Goal(Camera_light a,bool c,unsigned h):camera_light(a),climbing(c),lifter_height(h){}
Lights::Goal::Goal():Lights::Goal(Camera_light::OFF,false,0){}

Lights::Output::Output(bool c,double b):camera_light(c),blinky_light_info(b){}
Lights::Output::Output():Output(false,0.0){}

Lights::Status_detail::Status_detail(unsigned h,bool c,bool m,Alliance a,Time t):lifter_height(h),climbing(c),autonomous(m),alliance(a),now(t){}
Lights::Status_detail::Status_detail():Status_detail(0,false,false,Alliance::INVALID,0){}

Lights::Input::Input(bool a,Alliance al):autonomous(a),alliance(al){}
Lights::Input::Input():Input(false,Alliance::INVALID){}

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
	o<<"camera_light:"<<a.camera_light;
	o<<" climbing:"<<a.climbing;
	o<<" lifter_height:"<<a.lifter_height;
	return o<<")";
}

ostream& operator<<(ostream& o, Lights::Input a){
	o<<"(";
	o<<"autonomous:"<<a.autonomous;
	o<<"alliance:"<<a.alliance;
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
	return a.camera_light == b.camera_light && a.climbing == b.climbing && a.lifter_height == b.lifter_height;
}

bool operator<(Lights::Goal a,Lights::Goal b){
	CMP(camera_light)
	CMP(climbing)
	CMP(lifter_height)
	return 0;
}

bool operator!=(Lights::Goal a, Lights::Goal b){
	return !(a==b);
}

bool operator==(Lights::Input a,Lights::Input b){
	return a.autonomous == b.autonomous && a.alliance == b.alliance;
}

bool operator!=(Lights::Input a,Lights::Input b){
	return !(a==b);
}

bool operator<(Lights::Input a,Lights::Input b){
	CMP(autonomous)
	CMP(alliance)
	return false;
}

bool operator==(Lights::Input_reader, Lights::Input_reader){
	return true;
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

Lights::Input Lights::Input_reader::operator()(Robot_inputs r)const{
	return {r.robot_mode.autonomous,r.ds_info.alliance};
}

Robot_inputs Lights::Input_reader::operator()(Robot_inputs r, Lights::Input in)const{
	r.robot_mode.autonomous = in.autonomous;
	r.ds_info.alliance = in.alliance;
	return r;
}

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

void Lights::Estimator::update(Time now,Lights::Input in,Lights::Output){
	last.now = now;
	last.alliance = in.alliance;
	last.autonomous = in.autonomous;
}

Lights::Status_detail Lights::Estimator::get()const{
	return last;
}

set<Lights::Input> examples(Lights::Input*){ 
	return {
		{false,Alliance::INVALID},
		{false,Alliance::RED},
		{false,Alliance::BLUE},
		{true,Alliance::INVALID},
		{true,Alliance::RED},
		{true,Alliance::BLUE}
	};
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
		Lights::Goal{Lights::Camera_light::ON,false,0},
		Lights::Goal{Lights::Camera_light::ON,true,0},
		Lights::Goal{Lights::Camera_light::OFF,false,0},
		Lights::Goal{Lights::Camera_light::OFF,true,0}
	};
}

double encode_robot_status(Lights::Status_detail status){
	const unsigned INFO_TYPES = 4;//number of types of data for transmission
	
	const Time TRANSMIT_TIME = 1;//seconds
	const Time TIME_INTERVAL = TRANSMIT_TIME / (double)INFO_TYPES;//seconds to transmit each part of data

	double time_remainder = [&]{//time since the last transmit cycle began
		double remainder, integer;
		remainder = modf(status.now / TRANSMIT_TIME, &integer);
		return remainder * TRANSMIT_TIME;
	}();
	
	int to_transmit = [&]{//select one of the info types to transmit based on the time into the transmit cycle
		for(unsigned i = 1; i <= INFO_TYPES; i++){
			if(time_remainder < TIME_INTERVAL * (double)i + 1E-14){
				return i;
			}
		}
		nyi
	}();
	
	double out = [&]{
		switch(to_transmit){
			case 1:
				return Lights::blinky_light_transcriber.transcribe("climbing",status.climbing);
			case 2:
				return Lights::blinky_light_transcriber.transcribe("autonomous",status.autonomous);
			case 3:
				return Lights::blinky_light_transcriber.transcribe("alliance",status.alliance);
			case 4: 
				return Lights::blinky_light_transcriber.transcribe("lifter_height",status.lifter_height);
			default:
				nyi
		}
	}();
	return out;
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
	Lights::init_blinky_light_transcriber();
	{
		Lights a;
		Tester_mode mode;
		mode.check_outputs_exhaustive = false;
		tester(a,mode);
	}
	cout<<"\n\n=============================================\n\n";
	{//this generates example output values for debugging the blinky light arduino code
		cout<<"\n\n"<<Lights::blinky_light_transcriber<<"\n\n";
		unsigned lifter_height = 6;
		bool climbing = false;
		bool autonomous = true;
		Alliance alliance = Alliance::BLUE;
		
		Lights::Status_detail status =  {lifter_height, climbing, autonomous, alliance, 0.0};
		
		Lights::Goal goal = *examples((Lights::Goal*)nullptr).begin();
		
		cout<<"status:"<<status<<"\n";
		cout<<"{";
		for(Time t = 0; t < 2; t+= .01){
			if(t != 0) cout<<",";
			status.now = t;
			Lights::Output out = control(status,goal);
			cout<<out.blinky_light_info;
		}
		cout<<"}";
	}
}

#endif
