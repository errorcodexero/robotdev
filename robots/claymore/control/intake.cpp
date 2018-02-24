#include "intake.h"
#include <stdlib.h>
#include <cmath>

using namespace std;

#define INTAKE_ADDRESS_R 4
#define INTAKE_ADDRESS_L 3
#define L_INTAKE_POWER 1.0 //TODO tune this
#define R_INTAKE_POWER 1.0 //TODO tune this

Intake::Goal::Goal():left_(0.0),right_(0.0){}

double Intake::Goal::left()const {
	return left_;
}

double Intake::Goal::right()const {
	return right_;
}

Intake::Goal Intake::Goal::in() {
	Goal a;
	a.left_ = L_INTAKE_POWER;
	a.right_ = R_INTAKE_POWER;
	return a;
}

Intake::Goal Intake::Goal::in(double power) {
	Goal a;
	a.left_ = power;
	a.right_ = power;
	return a;
}

Intake::Goal Intake::Goal::off() {
	Goal a;
	a.left_ = 0.0;
	a.right_ = 0.0;
	return a;
}

Intake::Goal Intake::Goal::out() {
	Goal a;
	a.left_ = -L_INTAKE_POWER;
	a.right_ = -R_INTAKE_POWER;
	return a;
}

Intake::Goal Intake::Goal::out(double power) {
	Goal a;
	a.left_ = -power;
	a.right_ = -power;
	return a;
}

Intake::Goal Intake::Goal::absolute(double left, double right) {
	Goal a;
	a.left_ = left;
	a.right_ = right;
	return a;
}

Intake::Output::Output(double l, double r):left(l),right(r){}
Intake::Output::Output():Output(0.0, 0.0){}

ostream& operator<<(ostream& o, Intake::Goal a) {
	o << "left:" << a.left();
	o << " right:" << a.right();
	return o;
}

ostream& operator<<(ostream& o, Intake::Output a) {
	o << "left:" << a.left;
	o << " right:" << a.right;
	return o;
}

ostream& operator<<(ostream& o, Intake){ return o<<"Intake()";}

bool operator<(Intake::Goal a, Intake::Goal b) {
        if(a.left() < b.left()) return true;
        if(b.left() < a.left()) return false;
        if(a.right() < b.right()) return true;
        if(b.right() < a.right()) return false;
	return false;
}
bool operator==(Intake::Goal a, Intake::Goal b) {
	return a.left() == b.left() && a.right() == b.right();
}
bool operator!=(Intake::Goal a, Intake::Goal b) {
	return !(a==b);
}

bool operator<(Intake::Output a, Intake::Output b) {
        if(a.left < b.left) return true;
        if(b.left < a.left) return false;
        if(a.right < b.right) return true;
        if(b.right < a.right) return false;
	return false;
}
bool operator==(Intake::Output a, Intake::Output b) {
	return a.left == b.left && a.right == b.right;
}
bool operator!=(Intake::Output a, Intake::Output b) {
	return !(a==b);
}

bool operator==(Intake::Output_applicator, Intake::Output_applicator){return 1;}

bool operator==(Intake a, Intake b){ return (a.input_reader==b.input_reader && a.estimator==b.estimator && a.output_applicator==b.output_applicator);}
bool operator!=(Intake a, Intake b){ return !(a==b);}

Robot_outputs Intake::Output_applicator::operator()(Robot_outputs r, Intake::Output out)const{
	r.pwm[INTAKE_ADDRESS_R] = out.right;
	r.pwm[INTAKE_ADDRESS_L] = out.left;
	return r;
}

Intake::Output Intake::Output_applicator::operator()(Robot_outputs r)const{
	return Output(r.pwm[INTAKE_ADDRESS_L], r.pwm[INTAKE_ADDRESS_R]);
}

set<Intake::Goal> examples(Intake::Goal*){ 
	return {Intake::Goal::out(), Intake::Goal::off(), Intake::Goal::in()};
}

Intake::Output control(Intake::Status_detail, Intake::Goal goal){
	return Intake::Output(goal.left(), goal.right());
}

bool ready(Intake::Status, Intake::Goal){ return 1;}

#ifdef INTAKE_TEST
#include "formal.h"

int main(){
	Intake a;
	tester(a);
}

#endif
