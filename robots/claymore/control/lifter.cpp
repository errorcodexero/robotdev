#include "lifter.h"

using namespace std;

Robot_outputs Lifter::Output_applicator::operator()(Robot_outputs r, Lifter::Output const& out)const{
	nyi
	return r;
};

Lifter::Output Lifter::Output_applicator::operator()(Robot_outputs const& r){
	nyi
}

Robot_inputs Lifter::Input_reader::operator()(Robot_inputs r, Lifter::Input const& in)const{
	nyi
	return r;
}

Lifter::Input Lifter::Input_reader::operator()(Robot_inputs const& r){
	nyi
}

void Estimator::update(Time const&, Lifter::Input const&, Lifter::Output const&){
	nyi
}

Lifter::Status_detail Estimator::get()const{
	nyi
}

set<Lifter::Input> examples(Lifter::Input*){
	nyi
}

set<Lifter::Output> examples(Lifter::Output*){
	nyi
}

set<Lifter::Status_detail> examples(Lifter::Status_detail*){
	nyi
}

set<Lifter::Goal> examples(Lifter::Goal*){
	nyi
}

Lifter::Output control(Lifter::Status_detail const&,Lifter::Goal const&){
	nyi
}

Lifter::Status status(Lifter::Status_detail const&){
	nyi
}

bool ready(Lifter::Status_detail const&,Lifter::Goal const&){
	nyi
}

#ifdef LIFTER_TEST
#include "formal.h"

int main(){
	Lifter lifter;
	tester(lifter);
}

#endif
