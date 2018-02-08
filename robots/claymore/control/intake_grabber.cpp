#include "intake_grabber.h"

using namespace std;

#define INTAKE_L 2
#define INTAKE_R 3

#define ENCODER_L 1
#define ENCODER_R 2

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

Intake_grabber::Input::Input(int l, int r):ticks_l(l),ticks_r(r){}
Intake_grabber::Input::Input():Input(0,0){}

Intake_grabber::Status_detail::Status_detail(double l,double r):angle_l(l),angle_r(r){}
Intake_grabber::Status_detail::Status_detail():Status_detail(0.0,0.0){}

Intake_grabber::Estimator::Estimator():last(){}

std::set<Intake_grabber::Goal> examples(Intake_grabber::Goal*){
	return {Intake_grabber::Goal::open(),Intake_grabber::Goal::stop(),Intake_grabber::Goal::close(),Intake_grabber::Goal::go_to_angle(0.0,0.0)};
}

std::set<Intake_grabber::Output> examples(Intake_grabber::Output*){
	return {-MANUAL_INTAKE_GRABBER_POWER,0.0,MANUAL_INTAKE_GRABBER_POWER};
}

std::set<Intake_grabber::Input> examples(Intake_grabber::Input*){
	return {
		{0,0}
	};
}

std::ostream& operator<<(std::ostream& o,Intake_grabber::Input a){
	o<<"(";
	o<<"ticks_l:"<<a.ticks_l;
	o<<" ticks_r:"<<a.ticks_r;
	o<<")";
	return o;
}

std::set<Intake_grabber::Status_detail> examples(Intake_grabber::Status_detail*){
	return {{0.0,0.0}};
}

std::ostream& operator<<(std::ostream& o,Intake_grabber::Status_detail a){
	o<<"(";
	o<<"angle_l:"<<a.angle_l;
	o<<" angle_r:"<<a.angle_r;
	o<<")";
	return o;
}

#define CMP(VAR) \
	if(a.VAR < b.VAR) return true; \
	if(b.VAR < a.VAR) return false; 

	
bool operator<(Intake_grabber::Status_detail a,Intake_grabber::Status_detail b){
	CMP(angle_l)
	CMP(angle_r)
	return false;
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
bool operator==(Intake_grabber::Status_detail a,Intake_grabber::Status_detail b){
	return a.angle_l == b.angle_l && a.angle_r == b.angle_r;
}

bool operator<(Intake_grabber::Input a,Intake_grabber::Input b){
	CMP(ticks_l)
	CMP(ticks_r)
	return false;
}

bool operator==(Intake_grabber::Input a,Intake_grabber::Input b){
	return a.ticks_l == b.ticks_l && a.ticks_r == b.ticks_r;
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
	return {r.digital_io.encoder[ENCODER_L],r.digital_io.encoder[ENCODER_R]};
}

Robot_inputs Intake_grabber::Input_reader::operator()(Robot_inputs r, Intake_grabber::Input in) const{
	r.digital_io.encoder[ENCODER_L] = in.ticks_l;
	r.digital_io.encoder[ENCODER_R] = in.ticks_r;
	return r;
}

Robot_outputs Intake_grabber::Output_applicator::operator()(Robot_outputs r, Intake_grabber::Output o)const{
	r.pwm[INTAKE_L] = o;
	r.pwm[INTAKE_R] = o;
	return r;
}

Intake_grabber::Output Intake_grabber::Output_applicator::operator()(Robot_outputs const& r)const{
	return r.pwm[INTAKE_L];
}

void Intake_grabber::Estimator::update(Time time,Intake_grabber::Input input,Intake_grabber::Output output){
	//TODO
}

Intake_grabber::Status Intake_grabber::Estimator::get()const{
	return last;
}

Intake_grabber::Output control(Intake_grabber::Status status_detail,Intake_grabber::Goal goal){
	Intake_grabber::Status s = status(status_detail);
	if(ready(s, goal)){
		return {0.0};
	}
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
				double error = goal.target() - status_detail.angle_l;
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
	return true;//TODO
}

#ifdef INTAKE_GRABBER_TEST
#include "formal.h"

int main(){
	Intake_grabber a;
	tester(a);
}
#endif
