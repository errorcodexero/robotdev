#include "lifter.h"

using namespace std;

#define LIFTER_ADDRESS_L 2
#define LIFTER_ADDRESS_R 3 //TODO

#define LIFTER_POWER .30 //TODO tune

ostream& operator<<(ostream& o, Lifter::Goal::Mode a){
	#define X(MODE) if(a==Lifter::Goal::Mode::MODE) return o<<""#MODE;
	LIFTER_GOAL_MODES
	#undef X
	assert(0);
}

Lifter::Goal::Mode Lifter::Goal::mode()const{
	return mode_;
}

double Lifter::Goal::target()const{
	return target_;
}

double Lifter::Goal::tolerance()const{
	return tolerance_;
}

Lifter::Goal::Goal():mode_(Lifter::Goal::Mode::STOP),target_(0.0),tolerance_(0.0){}

Lifter::Goal Lifter::Goal::up(){
	Lifter::Goal a;
	a.mode_ = Lifter::Goal::Mode::UP;
	return a;
}

Lifter::Goal Lifter::Goal::stop(){
	Lifter::Goal a;
	a.mode_ = Lifter::Goal::Mode::STOP;
	return a;
}

Lifter::Goal Lifter::Goal::down(){
	Lifter::Goal a;
	a.mode_ = Lifter::Goal::Mode::DOWN;
	return a;
}

Lifter::Goal Lifter::Goal::go_to_height(double target, double tolerance){
	Lifter::Goal a;
	a.mode_ = Lifter::Goal::Mode::GO_TO_HEIGHT;
	a.target_ = target;
	a.tolerance_ = tolerance;
	return a;
}

#define CMP(VAR) \
	if(a.VAR < b.VAR) return true; \
	if(b.VAR < a.VAR) return false; 	

bool operator==(Lifter::Input const& a, Lifter::Input const& b){
	return true;
}

bool operator!=(Lifter::Input const& a, Lifter::Input const& b){
	return !(a==b);
}

bool operator<(Lifter::Input const& a,Lifter::Input const& b){
	return false;
}

ostream& operator<<(ostream& o, Lifter::Input const&){
	return o;
}

bool operator<(Lifter::Status const& a,Lifter::Status const& b){
	return false;
}

bool operator==(Lifter::Status const& a, Lifter::Status const& b){
	return true;
}

bool operator!=(Lifter::Status const& a, Lifter::Status const& b){
	return !(a==b);
}

ostream& operator<<(ostream& o, Lifter::Status const&){
	return o;
}

bool operator==(Lifter::Estimator const& a, Lifter::Estimator const& b){
	return true;
}

bool operator!=(Lifter::Estimator const& a, Lifter::Estimator const& b){
	return !(a==b);
}

bool operator<(Lifter::Goal const& a,Lifter::Goal const& b){
	if(a.mode() == b.mode() && a.mode() == Lifter::Goal::Mode::GO_TO_HEIGHT){
		CMP(target())
		CMP(tolerance())
	}
	return a.mode() < b.mode();
}

ostream& operator<<(ostream& o, Lifter::Goal const& a){
	o<<"Goal( "<<a.mode();
	if(a.mode() == Lifter::Goal::Mode::GO_TO_HEIGHT){
		o<<" "<<a.target();
		o<<" "<<a.tolerance();
	}
	o<<")";
	return o;
}

bool operator==(Lifter::Status_detail const&,Lifter::Status_detail const&){
	return true;
}

bool operator!=(Lifter::Status_detail const& a,Lifter::Status_detail const& b){
	return !(a==b);
}

bool operator<(Lifter::Status_detail const& a,Lifter::Status_detail const& b){
	return false;
}

ostream& operator<<(ostream& o, Lifter::Status_detail const& a){
	return o;
}

ostream& operator<<(ostream& o, Lifter const& a){
	return o<<"Lifter("<<a.estimator.get()<<")";
}

#undef CMP

Robot_outputs Lifter::Output_applicator::operator()(Robot_outputs r, Lifter::Output const& out)const{
	r.pwm[LIFTER_ADDRESS_L] = out;
	r.pwm[LIFTER_ADDRESS_R] = out;
	return r;
};

Lifter::Output Lifter::Output_applicator::operator()(Robot_outputs const& r)const{
	return r.pwm[LIFTER_ADDRESS_L]; //assuming that left and right sides are set to the same value
}

Robot_inputs Lifter::Input_reader::operator()(Robot_inputs r, Lifter::Input const&)const{
	return r;
}

Lifter::Input Lifter::Input_reader::operator()(Robot_inputs const&)const{
	return {};
}

void Lifter::Estimator::update(Time const&, Lifter::Input const&, Lifter::Output const&){
	//do nothing
}

Lifter::Status_detail Lifter::Estimator::get()const{
	return {};
}

set<Lifter::Input> examples(Lifter::Input*){
	return {Lifter::Input{}};
}

set<Lifter::Output> examples(Lifter::Output*){
	return {-LIFTER_POWER,0.0,LIFTER_POWER};
}

set<Lifter::Status_detail> examples(Lifter::Status_detail*){
	return {Lifter::Status_detail{}};
}

set<Lifter::Status> examples(Lifter::Status*){
	return {Lifter::Status{}};
}

set<Lifter::Goal> examples(Lifter::Goal*){
	return {Lifter::Goal::up(),Lifter::Goal::stop(),Lifter::Goal::down(),Lifter::Goal::go_to_height(0.0,0.0)};
}

Lifter::Output control(Lifter::Status_detail const&,Lifter::Goal const& goal){
	switch(goal.mode()){
		case Lifter::Goal::Mode::UP:
			return {LIFTER_POWER};
		case Lifter::Goal::Mode::STOP:
			return {0.0};
		case Lifter::Goal::Mode::DOWN:
			return {-LIFTER_POWER};
		case Lifter::Goal::Mode::GO_TO_HEIGHT:
			return {0.0};//TODO
		default:
			nyi
	}
}

Lifter::Status status(Lifter::Status_detail const&){
	return {};
}

bool ready(Lifter::Status const&,Lifter::Goal const&){
	return true;
}

#ifdef LIFTER_TEST
#include "formal.h"

int main(){
	Lifter lifter;
	tester(lifter);
}

#endif
