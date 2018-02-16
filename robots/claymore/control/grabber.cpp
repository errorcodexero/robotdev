#include "grabber.h"
#include <cmath>

using namespace std;

#define GRABBER_ADDRESS 2

#define ENCODER_ADDRESS 0

#define MANUAL_GRABBER_POWER .60 //TODO tune
#define AUTO_GRABBER_POWER .60 //TODO tune

GrabberController Grabber::grabber_controller;

ostream& operator<<(ostream& o, Grabber::Goal::Mode a){
	#define X(MODE) if(a==Grabber::Goal::Mode::MODE) return o<<""#MODE;
	GRABBER_GOAL_MODES
	#undef X
	assert(0);
}

ostream& operator<<(ostream& o, Grabber::Goal a){
	o<<"(";
	o<<"mode:"<<a.mode();
	if(a.mode() == Grabber::Goal::Mode::GO_TO_ANGLE){
		o<<" target:"<<a.target();
	}
	o<<")";
	return o;
}

Grabber::Goal::Mode Grabber::Goal::mode()const{
	return mode_;
}

double Grabber::Goal::target()const{
	return target_;
}

GrabberController::Preset Grabber::Goal::preset_target()const{
	return preset_target_;
}

Grabber::Goal::Goal():mode_(Grabber::Goal::Mode::STOP),target_(0.0),preset_target_(GrabberController::Preset::CLOSED){}

Grabber::Goal Grabber::Goal::open(){
	Grabber::Goal a;
	a.mode_ = Grabber::Goal::Mode::OPEN;
	return a;
}

Grabber::Goal Grabber::Goal::stop(){
	Grabber::Goal a;
	a.mode_ = Grabber::Goal::Mode::STOP;
	return a;
}

Grabber::Goal Grabber::Goal::close(){
	Grabber::Goal a;
	a.mode_ = Grabber::Goal::Mode::CLOSE;
	return a;
}

Grabber::Goal Grabber::Goal::go_to_angle(double target){
	Grabber::Goal a;
	a.mode_ = Grabber::Goal::Mode::GO_TO_ANGLE;
	a.target_ = target;
	return a;
}

Grabber::Goal Grabber::Goal::go_to_preset(GrabberController::Preset preset){
	Grabber::Goal a;
	a.mode_ = Grabber::Goal::Mode::GO_TO_PRESET;
	a.preset_target_ = preset;
	return a;
}

Grabber::Input::Input(int t):ticks(t){}
Grabber::Input::Input():Input(0){}

Grabber::Status_detail::Status_detail(double a):angle(a){}
Grabber::Status_detail::Status_detail():Status_detail(0.0){}

Grabber::Estimator::Estimator():last(){}

std::set<Grabber::Goal> examples(Grabber::Goal*){
	return {Grabber::Goal::open(),Grabber::Goal::stop(),Grabber::Goal::close(),Grabber::Goal::go_to_angle(0.0),Grabber::Goal::go_to_preset(GrabberController::Preset::CLOSED)};
}

std::set<Grabber::Output> examples(Grabber::Output*){
	return {-MANUAL_GRABBER_POWER,0.0,MANUAL_GRABBER_POWER};
}

std::set<Grabber::Input> examples(Grabber::Input*){
	return {
		{0},
		{1}
	};
}

std::ostream& operator<<(std::ostream& o,Grabber::Input a){
	o<<"(";
	o<<"ticks:"<<a.ticks;
	o<<"has_cube:"<<a.has_cube;
	o<<")";
	return o;
}

std::set<Grabber::Status_detail> examples(Grabber::Status_detail*){
	return {{0.0}};
}

std::ostream& operator<<(std::ostream& o,Grabber::Status_detail a){
	o<<"(";
	o<<"angle:"<<a.angle;
	o<<"time:"<<a.time;
	o<<"dt:"<<a.dt;
	o<<"has_cube:"<<a.has_cube;
	o<<")";
	return o;
}

#define CMP(VAR) \
	if(a.VAR < b.VAR) return true; \
	if(b.VAR < a.VAR) return false; 

bool operator<(Grabber::Status_detail a,Grabber::Status_detail b){
	CMP(angle)
	return false;
}

bool operator==(Grabber::Status_detail a,Grabber::Status_detail b){
	return a.angle == b.angle && a.time == b.time && a.dt == b.dt && a.has_cube == b.has_cube;
}

bool operator!=(Grabber::Status_detail a, Grabber::Status_detail b){
	return !(a==b);
}

bool operator<(Grabber::Goal a,Grabber::Goal b){
	CMP(mode())
	if(a.mode() == Grabber::Goal::Mode::GO_TO_ANGLE){
		CMP(target())
	}
	return false;
}

std::ostream& operator<<(std::ostream& o,Grabber const&){
	return o<<"Grabber()";
}

bool operator<(Grabber::Input a,Grabber::Input b){
	CMP(ticks)
	return false;
}

bool operator==(Grabber::Input a,Grabber::Input b){
	return a.ticks == b.ticks && a.has_cube == b.has_cube;
}

bool operator!=(Grabber::Input a, Grabber::Input b){
	return !(a==b);
}

bool operator==(Grabber::Estimator a, Grabber::Estimator b){
	return a.last==b.last;
}

bool operator!=(Grabber::Estimator a, Grabber::Estimator b){
	return !(a==b);
}

ostream& operator<<(ostream& o, Grabber::Estimator a){
	return o<<"(last:"<<a.last<<")";
}

bool operator==(Grabber,Grabber){
	return 1;
}

bool operator!=(Grabber a, Grabber b){
	return !(a==b);
}

#undef CMP

Grabber::Input Grabber::Input_reader::operator()(Robot_inputs const& r) const{
	return {
		r.digital_io.encoder[ENCODER_ADDRESS] ? *r.digital_io.encoder[ENCODER_ADDRESS] : 10000,
	};
}

Robot_inputs Grabber::Input_reader::operator()(Robot_inputs r, Grabber::Input in) const{
	r.digital_io.encoder[ENCODER_ADDRESS] = in.ticks;
	return r;
}

Robot_outputs Grabber::Output_applicator::operator()(Robot_outputs r, Grabber::Output o)const{
	r.pwm[GRABBER_ADDRESS] = o;
	return r;
}

Grabber::Output Grabber::Output_applicator::operator()(Robot_outputs const& r)const{
	return r.pwm[GRABBER_ADDRESS];
}

void Grabber::Estimator::update(Time time,Grabber::Input input,Grabber::Output output){
	const double TICKS_PER_REVOLUTION = 200.0;
	const double DEGREES_PER_TICK = 360.0 / TICKS_PER_REVOLUTION;
	last.angle = DEGREES_PER_TICK * input.ticks;

	last.dt = last.time - time;
	last.time = time;

	last.has_cube = input.has_cube;
}

Grabber::Status Grabber::Estimator::get()const{
	return last;
}

Grabber::Output control(Grabber::Status_detail status,Grabber::Goal goal){
	Grabber::Output out = 0.0;
	switch(goal.mode()){
		case Grabber::Goal::Mode::OPEN:
			out = MANUAL_GRABBER_POWER;
			break;
		case Grabber::Goal::Mode::STOP:
			Grabber::grabber_controller.idle(status.angle, status.time, status.dt);
			out = 0.0;
			break;
		case Grabber::Goal::Mode::CLOSE:
			out = -MANUAL_GRABBER_POWER;
			break;
		case Grabber::Goal::Mode::GO_TO_ANGLE:
			Grabber::grabber_controller.updateAngleOnChange(goal.target(), status.time);
			Grabber::grabber_controller.update(status.angle, status.time, status.dt, out);
			break;
		case Grabber::Goal::Mode::GO_TO_PRESET:	
			Grabber::grabber_controller.updateAngleOnChange(goal.preset_target(), status.time);
			Grabber::grabber_controller.update(status.angle, status.time, status.dt, out);
			break;
		default:
			assert(0);
	}
	return out;
}

Grabber::Status status(Grabber::Status s){
	return s;
}

bool ready(Grabber::Status status,Grabber::Goal goal){
	switch(goal.mode()){
		case Grabber::Goal::Mode::OPEN:
		case Grabber::Goal::Mode::CLOSE:
		case Grabber::Goal::Mode::STOP:
			return true;
		case Grabber::Goal::Mode::GO_TO_ANGLE:
		case Grabber::Goal::Mode::GO_TO_PRESET:
			return Grabber::grabber_controller.done();
		default:
			assert(0);
	}
}

#ifdef GRABBER_TEST
#include "formal.h"

int main(){
	Grabber a;
	tester(a);
}
#endif
