#include "intake_grabber.h"

using namespace std;

#define PISTON_LOC 2
#define INTAKE_GRABBER_LIMIT 5

Intake_grabber::Input::Input():enabled(false){}
Intake_grabber::Input::Input(bool e):enabled(e){}

Intake_grabber::Estimator::Estimator(){
	last = Status_detail::CLOSED;
}

std::set<Intake_grabber::Goal> examples(Intake_grabber::Goal*){
	return {
		#define X(NAME) Intake_grabber::Goal::NAME,
		INTAKE_GRABBER_GOALS(X)
		#undef X
	};
}

std::set<Intake_grabber::Output> examples(Intake_grabber::Output*){
	return {
		#define X(NAME) Intake_grabber::Output::NAME,
		INTAKE_GRABBER_OUTPUTS(X)
		#undef X
	};
}

std::set<Intake_grabber::Input> examples(Intake_grabber::Input*){
	return {
		{false},
		{true}
	};
}

std::ostream& operator<<(std::ostream& o,Intake_grabber::Input a){
	return o<<"Input(enabled:"<<a.enabled<<")";
}

ostream& operator<<(ostream& o,Intake_grabber::Output a){
	#define X(A) if(a==Intake_grabber::Output::A) return o<<""#A;
	INTAKE_GRABBER_OUTPUTS(X)
	#undef X
	assert(0);
}

std::set<Intake_grabber::Status_detail> examples(Intake_grabber::Status_detail*){
	return {Intake_grabber::Status_detail::CLOSED,Intake_grabber::Status_detail::CLOSING,Intake_grabber::Status_detail::OPENING,Intake_grabber::Status_detail::OPEN};
}

std::ostream& operator<<(std::ostream& o,Intake_grabber::Goal g){
	#define X(name) if(g==Intake_grabber::Goal::name) return o<<""#name;
	INTAKE_GRABBER_GOALS(X)
	#undef X
	assert(0);
}


std::ostream& operator<<(std::ostream& o,Intake_grabber::Status_detail a){
	#define X(STATUS) if(a==Intake_grabber::Status_detail::STATUS) return o<<""#STATUS;
	X(CLOSED)
	X(CLOSING)
	X(OPENING)
	X(OPEN)
	#undef X
	assert(0);
}

std::ostream& operator<<(std::ostream& o,Intake_grabber const&){
	return o<<"Intake_grabber()";
}

bool operator<(Intake_grabber::Input a,Intake_grabber::Input b){
	return !a.enabled && b.enabled;
}
bool operator==(Intake_grabber::Input a,Intake_grabber::Input b){
	return a.enabled == b.enabled;
}
bool operator!=(Intake_grabber::Input a, Intake_grabber::Input b){ return !(a==b); }

bool operator==(Intake_grabber::Estimator a, Intake_grabber::Estimator b){ return a.last==b.last && a.state_timer==b.state_timer; }

bool operator!=(Intake_grabber::Estimator a, Intake_grabber::Estimator b){ return !(a==b); }
ostream& operator<<(ostream& o, Intake_grabber::Estimator a){
	return o<<"Estimator(last:"<<a.last<<" state_timer:"<<a.state_timer<<")";
}

bool operator==(Intake_grabber,Intake_grabber){ return 1; }
bool operator!=(Intake_grabber a, Intake_grabber b){ return !(a==b); }

Intake_grabber::Input Intake_grabber::Input_reader::operator()(Robot_inputs const& r) const{
	return {r.robot_mode.enabled};
}

Robot_inputs Intake_grabber::Input_reader::operator()(Robot_inputs r, Intake_grabber::Input in) const{
	r.robot_mode.enabled=in.enabled;
	return r;
}

Robot_outputs Intake_grabber::Output_applicator::operator()(Robot_outputs r, Intake_grabber::Output o)const{
	r.solenoid[PISTON_LOC] = o == Intake_grabber::Output::OPEN;
	return r;
}

Intake_grabber::Output Intake_grabber::Output_applicator::operator()(Robot_outputs const& r)const{
	return r.solenoid[PISTON_LOC] ? Output::OPEN : Output::CLOSE;
}

void Intake_grabber::Estimator::update(Time time,Intake_grabber::Input input,Intake_grabber::Output output){
	switch(output){
		case Intake_grabber::Output::OPEN:
			if(last == Status::OPENING){
				state_timer.update(time,input.enabled);
			} else if(last != Status::OPEN){ 
				const Time OPEN_TIME = .6;//seconds. tested
				last = Status::OPENING;
				state_timer.set(OPEN_TIME);
			}
			if(state_timer.done() || last == Status::OPEN) {
				last = Status::OPEN;
			}
			break;
		case Intake_grabber::Output::CLOSE:
			/*
			if(last == Status::CLOSING){
				state_timer.update(time,input.enabled);
			} else if(last != Status::CLOSED){ 
				const Time CLOSE_TIME = .6;//seconds. tested
				last = Status::CLOSING;
				state_timer.set(CLOSE_TIME);
			}
			if(state_timer.done() || last == Status::CLOSED) { 
				last = Status::CLOSED;
			}*/
			if(last != Status::CLOSED) last = Status::CLOSING;
			if(last == Status::CLOSED) last = Status::CLOSED;
			break;
		default:
			assert(0);
	}
}

Intake_grabber::Status Intake_grabber::Estimator::get()const{
	return last;
}

Intake_grabber::Output control(Intake_grabber::Status status,Intake_grabber::Goal goal){
	switch(goal){
		case Intake_grabber::Goal::OPEN:
			return Intake_grabber::Output::OPEN;
		case Intake_grabber::Goal::CLOSE:
			return Intake_grabber::Output::CLOSE;
		case Intake_grabber::Goal::X:
			switch(status){
				case Intake_grabber::Status::CLOSED:
				case Intake_grabber::Status::CLOSING:
					return Intake_grabber::Output::CLOSE;
				case Intake_grabber::Status::OPENING:
				case Intake_grabber::Status::OPEN:
					return Intake_grabber::Output::OPEN;
				default:
					assert(0);
			}
		default:
			assert(0);
	}
}

Intake_grabber::Status status(Intake_grabber::Status s){
	return s;
}

bool ready(Intake_grabber::Status status,Intake_grabber::Goal goal){
	switch(goal){
		case Intake_grabber::Goal::CLOSE:
			return status == Intake_grabber::Status::CLOSED;
		case Intake_grabber::Goal::OPEN:
			return status == Intake_grabber::Status::OPEN;
		case Intake_grabber::Goal::X:
			return 1;
		default:
			assert(0);
	}
	return false;
}

#ifdef INTAKE_GRABBER_TEST
#include "formal.h"
int main(){
	{
		Intake_grabber a;
		tester(a);
	}
	{
		/*		
		Intake_grabber a;
		Intake_grabber::Goal goal = Intake_grabber::Goal::UP;

		const bool ENABLED = true;	
		for(Time t: range(100)){
			Intake_grabber::Status_detail status = a.estimator.get();
			Intake_grabber::Output out = control(status,goal);

			cout<<"t:"<<t<<"\tgoal:"<<goal<<"\tstatus:"<<status<<"\n";

			a.estimator.update(t,Intake_grabber::Input{ENABLED,false},out);
			if(ready(status,goal)){
				cout<<"Goal "<<goal<<" reached. Finishing\n";
				break;
			}
		}

		goal = Intake_grabber::Goal::DOWN;
		
		for(Time t: range(100)){
			Intake_grabber::Status_detail status = a.estimator.get();
			Intake_grabber::Output out = control(status,goal);

			cout<<"t:"<<t<<"\tgoal:"<<goal<<"\tstatus:"<<status<<"\n";

			a.estimator.update(t,Intake_grabber::Input{ENABLED,t>2},out);
			if(ready(status,goal)){
				cout<<"Goal "<<goal<<" reached. Finishing\n";
				break;
			}

		}
		*/
	}
}
#endif
