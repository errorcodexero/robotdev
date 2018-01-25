#include "intake_actuator.h"

using namespace std;

#define PISTON_LOC 2
#define INTAKE_ACTUATOR_LIMIT 5

Intake_actuator::Input::Input():enabled(false){}
Intake_actuator::Input::Input(bool e):enabled(e){}

Intake_actuator::Estimator::Estimator(){
	last = Status_detail::CLOSED;
}

std::set<Intake_actuator::Goal> examples(Intake_actuator::Goal*){
	return {
		#define X(NAME) Intake_actuator::Goal::NAME,
		INTAKE_ACTUATOR_GOALS(X)
		#undef X
	};
}

std::set<Intake_actuator::Output> examples(Intake_actuator::Output*){
	return {
		#define X(NAME) Intake_actuator::Output::NAME,
		INTAKE_ACTUATOR_OUTPUTS(X)
		#undef X
	};
}

std::set<Intake_actuator::Input> examples(Intake_actuator::Input*){
        return {
                {false},
		{true}
        };
}

std::ostream& operator<<(std::ostream& o,Intake_actuator::Input a){
        return o<<"Input(enabled:"<<a.enabled<<")";
}

ostream& operator<<(ostream& o,Intake_actuator::Output a){
	#define X(A) if(a==Intake_actuator::Output::A) return o<<""#A;
	INTAKE_ACTUATOR_OUTPUTS(X)
	#undef X
	assert(0);
}

std::set<Intake_actuator::Status_detail> examples(Intake_actuator::Status_detail*){
	return {Intake_actuator::Status_detail::CLOSED,Intake_actuator::Status_detail::CLOSING,Intake_actuator::Status_detail::OPENING,Intake_actuator::Status_detail::OPEN};
}

std::ostream& operator<<(std::ostream& o,Intake_actuator::Goal g){
	#define X(name) if(g==Intake_actuator::Goal::name) return o<<""#name;
	INTAKE_ACTUATOR_GOALS(X)
	#undef X
	assert(0);
}


std::ostream& operator<<(std::ostream& o,Intake_actuator::Status_detail a){
	#define X(STATUS) if(a==Intake_actuator::Status_detail::STATUS) return o<<""#STATUS;
	X(CLOSED)
	X(CLOSING)
	X(OPENING)
	X(OPEN)
	#undef X
	assert(0);
}

std::ostream& operator<<(std::ostream& o,Intake_actuator const&){
	return o<<"Intake_actuator()";
}

bool operator<(Intake_actuator::Input a,Intake_actuator::Input b){
        return !a.enabled && b.enabled;
}
bool operator==(Intake_actuator::Input a,Intake_actuator::Input b){
        return a.enabled == b.enabled;
}
bool operator!=(Intake_actuator::Input a, Intake_actuator::Input b){ return !(a==b); }

bool operator==(Intake_actuator::Estimator a, Intake_actuator::Estimator b){ return a.last==b.last && a.state_timer==b.state_timer; }

bool operator!=(Intake_actuator::Estimator a, Intake_actuator::Estimator b){ return !(a==b); }
ostream& operator<<(ostream& o, Intake_actuator::Estimator a){
	return o<<"Estimator(last:"<<a.last<<" state_timer:"<<a.state_timer<<")";
}

bool operator==(Intake_actuator,Intake_actuator){ return 1; }
bool operator!=(Intake_actuator a, Intake_actuator b){ return !(a==b); }

Intake_actuator::Input Intake_actuator::Input_reader::operator()(Robot_inputs const& r) const{
	return {r.robot_mode.enabled};
}

Robot_inputs Intake_actuator::Input_reader::operator()(Robot_inputs r, Intake_actuator::Input in) const{
	r.robot_mode.enabled=in.enabled;
	return r;
}

Robot_outputs Intake_actuator::Output_applicator::operator()(Robot_outputs r, Intake_actuator::Output o)const{
	r.solenoid[PISTON_LOC] = o == Intake_actuator::Output::OPEN;
	return r;
}

Intake_actuator::Output Intake_actuator::Output_applicator::operator()(Robot_outputs const& r)const{
	return r.solenoid[PISTON_LOC] ? Output::OPEN : Output::CLOSE;
}

void Intake_actuator::Estimator::update(Time time,Intake_actuator::Input input,Intake_actuator::Output output){
	switch(output){
		case Intake_actuator::Output::OPEN:
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
		case Intake_actuator::Output::CLOSE:
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

Intake_actuator::Status Intake_actuator::Estimator::get()const{
	return last;
}

Intake_actuator::Output control(Intake_actuator::Status status,Intake_actuator::Goal goal){
	switch(goal){
		case Intake_actuator::Goal::OPEN:
			return Intake_actuator::Output::OPEN;
		case Intake_actuator::Goal::CLOSE:
			return Intake_actuator::Output::CLOSE;
		case Intake_actuator::Goal::X:
			switch(status){
				case Intake_actuator::Status::CLOSED:
				case Intake_actuator::Status::CLOSING:
					return Intake_actuator::Output::CLOSE;
				case Intake_actuator::Status::OPENING:
				case Intake_actuator::Status::OPEN:
					return Intake_actuator::Output::OPEN;
				default:
					assert(0);
			}
		default:
			assert(0);
	}
}

Intake_actuator::Status status(Intake_actuator::Status s){
	return s;
}

bool ready(Intake_actuator::Status status,Intake_actuator::Goal goal){
	switch(goal){
		case Intake_actuator::Goal::CLOSE:
			return status == Intake_actuator::Status::CLOSED;
		case Intake_actuator::Goal::OPEN:
			return status == Intake_actuator::Status::OPEN;
		case Intake_actuator::Goal::X:
			return 1;
		default:
			assert(0);
	}
	return false;
}

#ifdef INTAKE_ACTUATOR_TEST
#include "formal.h"
int main(){
	{
		Intake_actuator a;
		tester(a);
	}
	{
		/*		
		Intake_actuator a;
		Intake_actuator::Goal goal = Intake_actuator::Goal::UP;

		const bool ENABLED = true;	
		for(Time t: range(100)){
			Intake_actuator::Status_detail status = a.estimator.get();
			Intake_actuator::Output out = control(status,goal);

			cout<<"t:"<<t<<"\tgoal:"<<goal<<"\tstatus:"<<status<<"\n";

			a.estimator.update(t,Intake_actuator::Input{ENABLED,false},out);
			if(ready(status,goal)){
				cout<<"Goal "<<goal<<" reached. Finishing\n";
				break;
			}
		}

		goal = Intake_actuator::Goal::DOWN;
		
		for(Time t: range(100)){
			Intake_actuator::Status_detail status = a.estimator.get();
			Intake_actuator::Output out = control(status,goal);

			cout<<"t:"<<t<<"\tgoal:"<<goal<<"\tstatus:"<<status<<"\n";

			a.estimator.update(t,Intake_actuator::Input{ENABLED,t>2},out);
			if(ready(status,goal)){
				cout<<"Goal "<<goal<<" reached. Finishing\n";
				break;
			}

		}
		*/
	}
}
#endif
