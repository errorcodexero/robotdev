#include "intake_grabber.h"
#include <cmath>

using namespace std;

#define INTAKE_ADDRESS 2

#define ENCODER_ADDRESS 0

#define MANUAL_INTAKE_GRABBER_POWER .60 //TODO tune
#define AUTO_INTAKE_GRABBER_POWER .60 //TODO tune

ostream& operator<<(ostream& o, Intake_grabber::Goal::Mode a){
	#define X(MODE) if(a==Intake_grabber::Goal::Mode::MODE) return o<<""#MODE;
	INTAKE_GRABBER_GOAL_MODES
	#undef X
	assert(0);
}

ostream& operator<<(ostream& o, Intake_grabber::Goal a){
	o<<"(";
	o<<"mode:"<<a.mode();
	if(a.mode() == Intake_grabber::Goal::Mode::GO_TO_ANGLE){
		o<<" target:"<<a.target();
		o<<" tolerance:"<<a.tolerance();
	}
	o<<")";
	return o;
}

Intake_grabber::Goal::Mode Intake_grabber::Goal::mode()const{
	return mode_;
}

double Intake_grabber::Goal::target()const{
	return target_;
}

double Intake_grabber::Goal::tolerance()const{
	return tolerance_;
}

Intake_grabber::Goal::Goal():mode_(Intake_grabber::Goal::Mode::STOP),target_(0.0),tolerance_(0.0){}

Intake_grabber::Goal Intake_grabber::Goal::open(){
	Intake_grabber::Goal a;
	a.mode_ = Intake_grabber::Goal::Mode::OPEN;
	return a;
}

Intake_grabber::Goal Intake_grabber::Goal::stop(){
	Intake_grabber::Goal a;
	a.mode_ = Intake_grabber::Goal::Mode::STOP;
	return a;
}

Intake_grabber::Goal Intake_grabber::Goal::close(){
	Intake_grabber::Goal a;
	a.mode_ = Intake_grabber::Goal::Mode::CLOSE;
	return a;
}

Intake_grabber::Goal Intake_grabber::Goal::go_to_angle(double target, double tolerance){
	Intake_grabber::Goal a;
	a.mode_ = Intake_grabber::Goal::Mode::GO_TO_ANGLE;
	a.target_ = target;
	a.tolerance_ = tolerance;
	return a;
}

Intake_grabber::Input::Input(int t):ticks(t){}
Intake_grabber::Input::Input():Input(0){}

Intake_grabber::Status_detail::Status_detail(double a):angle(a){}
Intake_grabber::Status_detail::Status_detail():Status_detail(0.0){}

Intake_grabber::Estimator::Estimator():last(){}

std::set<Intake_grabber::Goal> examples(Intake_grabber::Goal*){
	return {Intake_grabber::Goal::open(),Intake_grabber::Goal::stop(),Intake_grabber::Goal::close(),Intake_grabber::Goal::go_to_angle(0.0,0.0)};
}

std::set<Intake_grabber::Output> examples(Intake_grabber::Output*){
	return {-MANUAL_INTAKE_GRABBER_POWER,0.0,MANUAL_INTAKE_GRABBER_POWER};
}

std::set<Intake_grabber::Input> examples(Intake_grabber::Input*){
	return {
		{0},
		{1}
	};
}

std::ostream& operator<<(std::ostream& o,Intake_grabber::Input a){
	o<<"(";
	o<<"ticks:"<<a.ticks;
	o<<")";
	return o;
}

std::set<Intake_grabber::Status_detail> examples(Intake_grabber::Status_detail*){
	return {{0.0}};
}

std::ostream& operator<<(std::ostream& o,Intake_grabber::Status_detail a){
	o<<"(";
	o<<"angle:"<<a.angle;
	o<<")";
	return o;
}

#define CMP(VAR) \
	if(a.VAR < b.VAR) return true; \
	if(b.VAR < a.VAR) return false; 

bool operator<(Intake_grabber::Status_detail a,Intake_grabber::Status_detail b){
	CMP(angle)
	return false;
}

bool operator==(Intake_grabber::Status_detail a,Intake_grabber::Status_detail b){
	return a.angle == b.angle;
}

bool operator!=(Intake_grabber::Status_detail a, Intake_grabber::Status_detail b){
	return !(a==b);
}

bool operator<(Intake_grabber::Goal a,Intake_grabber::Goal b){
	CMP(mode())
	if(a.mode() == Intake_grabber::Goal::Mode::GO_TO_ANGLE){
		CMP(target())
		CMP(tolerance())
	}
	return false;
}

std::ostream& operator<<(std::ostream& o,Intake_grabber const&){
	return o<<"Intake_grabber()";
}

bool operator<(Intake_grabber::Input a,Intake_grabber::Input b){
	CMP(ticks)
	return false;
}

bool operator==(Intake_grabber::Input a,Intake_grabber::Input b){
	return a.ticks == b.ticks;
}

bool operator!=(Intake_grabber::Input a, Intake_grabber::Input b){
	return !(a==b);
}

bool operator==(Intake_grabber::Estimator a, Intake_grabber::Estimator b){
	return a.last==b.last;
}

bool operator!=(Intake_grabber::Estimator a, Intake_grabber::Estimator b){
	return !(a==b);
}

ostream& operator<<(ostream& o, Intake_grabber::Estimator a){
	return o<<"(last:"<<a.last<<")";
}

bool operator==(Intake_grabber,Intake_grabber){
	return 1;
}

bool operator!=(Intake_grabber a, Intake_grabber b){
	return !(a==b);
}

#undef CMP

Intake_grabber::Input Intake_grabber::Input_reader::operator()(Robot_inputs const& r) const{
	return {
		r.digital_io.encoder[ENCODER_ADDRESS] ? *r.digital_io.encoder[ENCODER_ADDRESS] : 10000,
	};
}

Robot_inputs Intake_grabber::Input_reader::operator()(Robot_inputs r, Intake_grabber::Input in) const{
	r.digital_io.encoder[ENCODER_ADDRESS] = in.ticks;
	return r;
}

Robot_outputs Intake_grabber::Output_applicator::operator()(Robot_outputs r, Intake_grabber::Output o)const{
	r.pwm[INTAKE_ADDRESS] = o;
	return r;
}

Intake_grabber::Output Intake_grabber::Output_applicator::operator()(Robot_outputs const& r)const{
	return r.pwm[INTAKE_ADDRESS];
}

void Intake_grabber::Estimator::update(Time time,Intake_grabber::Input input,Intake_grabber::Output output){
	//TODO
}

Intake_grabber::Status Intake_grabber::Estimator::get()const{
	return last;
}

Intake_grabber::Output control(Intake_grabber::Status status_detail,Intake_grabber::Goal goal){
	switch(goal.mode()){
		case Intake_grabber::Goal::Mode::OPEN:
			return {MANUAL_INTAKE_GRABBER_POWER};
		case Intake_grabber::Goal::Mode::STOP:
			return {0.0};
		case Intake_grabber::Goal::Mode::CLOSE:
			return {-MANUAL_INTAKE_GRABBER_POWER};
		case Intake_grabber::Goal::Mode::GO_TO_ANGLE:
			{
				//TODO implement full PID control
				double error = goal.target() - status_detail.angle;
				const double P = 0.5;
				return clip(AUTO_INTAKE_GRABBER_POWER * error * P);
			}
		default:
			nyi
	}
}

Intake_grabber::Status status(Intake_grabber::Status s){
	return s;
}

bool ready(Intake_grabber::Status status,Intake_grabber::Goal goal){
	switch(goal.mode()){
		case Intake_grabber::Goal::Mode::OPEN:
		case Intake_grabber::Goal::Mode::CLOSE:
		case Intake_grabber::Goal::Mode::STOP:
			return true;
		case Intake_grabber::Goal::Mode::GO_TO_ANGLE:
			return fabs(goal.target() - status.angle) < 1;//TODO
		default:
			nyi
	}
}

#ifdef INTAKE_GRABBER_TEST
#include "formal.h"

int main(){
	Intake_grabber a;
	tester(a);
}
#endif