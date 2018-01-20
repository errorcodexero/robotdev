#include "lights.h"
#include <stdlib.h>
#include <cmath>

using namespace std;

#define CAMERA_LIGHT_ADDRESS 4

Lights::Goal::Goal(Camera_light a):camera_light(a){}
Lights::Goal::Goal():Lights::Goal(Camera_light::OFF){}

ostream& operator<<(ostream& o, Lights::Camera_light a){
	#define X(name) if(a==Lights::Camera_light::name)return o<<""#name;
	X(OFF) X(ON)
	#undef X
	nyi
}

ostream& operator<<(ostream& o, Lights::Goal a){
	o<<"(";
	o<<" camera_light:"<<a.camera_light;
	return o<<")";
}

ostream& operator<<(ostream& o, Lights){ return o<<"Lights()";}

bool operator==(Lights::Goal a,Lights::Goal b){
	return a.camera_light == b.camera_light;
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

Lights::Output Lights::Output_applicator::operator()(Robot_outputs r)const{
	Output out;
	if(r.digital_io[CAMERA_LIGHT_ADDRESS]==Digital_out::zero()) out.camera_light = Lights::Camera_light::OFF;
	else if(r.digital_io[CAMERA_LIGHT_ADDRESS]==Digital_out::one()) out.camera_light = Lights::Camera_light::ON;
	else nyi
	return out;
}

Robot_outputs Lights::Output_applicator::operator()(Robot_outputs r, Lights::Output out)const{
	r.digital_io[CAMERA_LIGHT_ADDRESS] = (out.camera_light == Lights::Camera_light::ON) ? Digital_out::one() : Digital_out::zero();
	return r;
}

Lights::Estimator::Estimator(){}

void Lights::Estimator::update(Time,Input,Output){}

Lights::Status_detail Lights::Estimator::get()const{
	return {};
}

set<Lights::Goal> examples(Lights::Goal*){ 
	return {
		Lights::Goal{Lights::Camera_light::ON},
		Lights::Goal{Lights::Camera_light::OFF}
	};
}

Lights::Output control(Lights::Status_detail, Lights::Goal goal){
	return goal;
}

bool ready(Lights::Status, Lights::Goal){ return 1;}

#ifdef LIGHTS_TEST
#include "formal.h"

int main(){
	Lights a;
	tester(a);
}

#endif
