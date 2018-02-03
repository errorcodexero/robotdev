#include "lights.h"
#include <stdlib.h>
#include <cmath>

using namespace std;

#define CAMERA_LIGHT_ADDRESS 4
#define BLINKY_LIGHT_INFO_ADDRESS 5

Lights::Goal::Goal(Camera_light a,bool c,unsigned h):camera_light(a),climbing(c),lifter_height(h){}
Lights::Goal::Goal():Lights::Goal(Camera_light::OFF,false,0){}

Lights::Output::Output(bool c,vector<byte> b):camera_light(c),blinky_light_info(b){}
Lights::Output::Output():Output(false,{}){}

Lights::Status_detail::Status_detail(unsigned h,bool c,bool m,bool e,Alliance a,Time t):lifter_height(h),climbing(c),autonomous(m),enabled(e),alliance(a),now(t){}
Lights::Status_detail::Status_detail():Status_detail(0,false,false,false,Alliance::INVALID,0){}

Lights::Input::Input(bool a,bool e,Alliance al):autonomous(a),enabled(e),alliance(al){}
Lights::Input::Input():Input(false,false,Alliance::INVALID){}

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
	o<<" enabled:"<<a.enabled;
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
	o<<" enabled:"<<a.enabled;
	o<<" alliance:"<<a.alliance;
	return o<<")";
}

ostream& operator<<(ostream& o, Lights){ return o<<"Lights()";}

#define CMP(ELEM) \
	if(a.ELEM < b.ELEM) return true; \
	if(b.ELEM < a.ELEM) return false;

bool operator==(Lights::Status_detail a,Lights::Status_detail b){
	return a.lifter_height == b.lifter_height && a.climbing == b.climbing && a.autonomous == b.autonomous && a.enabled == b.enabled && a.alliance == b.alliance && a.now == b.now;
}

bool operator!=(Lights::Status_detail a,Lights::Status_detail b){
	return !(a==b);
}

bool operator<(Lights::Status_detail a,Lights::Status_detail b){
	CMP(lifter_height)
	CMP(climbing)
	CMP(autonomous)
	CMP(alliance)
	CMP(enabled)
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
	return a.autonomous == b.autonomous && a.enabled == b.enabled && a.alliance == b.alliance;
}

bool operator!=(Lights::Input a,Lights::Input b){
	return !(a==b);
}

bool operator<(Lights::Input a,Lights::Input b){
	CMP(autonomous)
	CMP(alliance)
	CMP(enabled)
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
	return {r.robot_mode.autonomous,r.robot_mode.enabled,r.ds_info.alliance};
}

Robot_inputs Lights::Input_reader::operator()(Robot_inputs r, Lights::Input in)const{
	r.robot_mode.autonomous = in.autonomous;
	r.ds_info.alliance = in.alliance;
	r.robot_mode.enabled = in.enabled;
	return r;
}

Lights::Output Lights::Output_applicator::operator()(Robot_outputs r)const{
	Output out;
	out.camera_light = r.digital_io[CAMERA_LIGHT_ADDRESS] == Digital_out::one();
	out.blinky_light_info = r.i2c.data;
	return out;
}

Robot_outputs Lights::Output_applicator::operator()(Robot_outputs r, Lights::Output out)const{
	r.digital_io[CAMERA_LIGHT_ADDRESS] = out.camera_light ? Digital_out::one() : Digital_out::zero();
	r.i2c.data = out.blinky_light_info;
	return r;
}

Lights::Estimator::Estimator(){}

void Lights::Estimator::update(Time now,Lights::Input in,Lights::Output){
	last.now = now;
	last.alliance = in.alliance;
	last.enabled = in.enabled;
	last.autonomous = in.autonomous;
}

Lights::Status_detail Lights::Estimator::get()const{
	return last;
}

set<Lights::Input> examples(Lights::Input*){ 
	set<Lights::Input> s;
	vector<bool> bools = {false,true};
	vector<Alliance> alliances = {Alliance::RED,Alliance::BLUE,Alliance::INVALID};
	for(bool a: bools){
		for(bool e: bools){
			for(Alliance al: alliances){
				s.insert({a,e,al});
			}
		}
	}
	return s;
}

set<Lights::Status_detail> examples(Lights::Status_detail*){ 
	set<Lights::Status_detail> s;
	vector<bool> bools = {false,true};
	vector<Alliance> alliances = {Alliance::RED,Alliance::BLUE,Alliance::INVALID};
	for(bool c: bools){
		for(bool m: bools){
			for(bool e: bools){
				for(Alliance a: alliances){
					s.insert({0,c,m,e,a,0.0});
				}
			}
		}
	}
	return s;
}

set<Lights::Output> examples(Lights::Output*){ 
	set<Lights::Output> s;
	vector<bool> bools = {false,true};
	set<Lights::Status_detail> statuses = examples((Lights::Status_detail*)nullptr);
	set<Lights::Goal> goals = examples((Lights::Goal*)nullptr);
	for(bool c: bools){
		for(Lights::Status_detail status: statuses){
			for(Lights::Goal goal: goals){
				s.insert({c,vector<byte>{goal.climbing, status.autonomous, status.enabled, (byte)static_cast<int>(status.alliance), (byte)goal.lifter_height}});
			}
		}
	}
	return s;
}

set<Lights::Goal> examples(Lights::Goal*){ 
	return {
		Lights::Goal{Lights::Camera_light::ON,false,0},
		Lights::Goal{Lights::Camera_light::ON,true,0},
		Lights::Goal{Lights::Camera_light::OFF,false,0},
		Lights::Goal{Lights::Camera_light::OFF,true,0}
	};
}

Lights::Output control(Lights::Status status, Lights::Goal goal){
	Lights::Output out;
	out.camera_light = goal.camera_light == Lights::Camera_light::ON;
	out.blinky_light_info = {goal.climbing, status.autonomous, status.enabled, (byte)static_cast<int>(status.alliance), (byte)goal.lifter_height}; //encode_robot_status(status);
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
