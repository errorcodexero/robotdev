#ifndef	INTAKE_ACTUATOR_H
#define INTAKE_ACTUATOR_H

#include "../util/interface.h"
#include <set>
#include "../util/countdown_timer.h"
#include "nop.h"

struct Intake_actuator{
	#define INTAKE_ACTUATOR_GOALS(X) X(CLOSE) X(OPEN) X(X)
	enum class Goal{
		#define X(NAME) NAME,
		INTAKE_ACTUATOR_GOALS(X)
		#undef X
	};

	#define INTAKE_ACTUATOR_OUTPUTS(X) X(CLOSE) X(OPEN)
	enum class Output{
		#define X(NAME) NAME,
		INTAKE_ACTUATOR_OUTPUTS(X)
		#undef X
	};
	
	struct Input{
		bool enabled;
		Input();
		Input(bool);
	};

	enum class Status_detail{CLOSED,CLOSING,OPENING,OPEN};
	
	typedef Status_detail Status;
	
	struct Input_reader{
		Input operator()(Robot_inputs const&)const;
		Robot_inputs operator()(Robot_inputs,Input)const;
	};

	struct Output_applicator{
		Robot_outputs operator()(Robot_outputs,Output)const;
		Output operator()(Robot_outputs const&)const;
	};

	struct Estimator{
		Status_detail last;
		Countdown_timer state_timer;		

		void update(Time,Input,Output);
		Status_detail get()const;
		Estimator();
	};

	Input_reader input_reader;
	Output_applicator output_applicator;
	Estimator estimator;
};

std::set<Intake_actuator::Goal> examples(Intake_actuator::Goal*);
std::set<Intake_actuator::Input> examples(Intake_actuator::Input*);
std::set<Intake_actuator::Output> examples(Intake_actuator::Output*);
std::set<Intake_actuator::Status_detail> examples(Intake_actuator::Status_detail*);

std::ostream& operator<<(std::ostream&,Intake_actuator::Goal);
std::ostream& operator<<(std::ostream&,Intake_actuator::Input);
std::ostream& operator<<(std::ostream&,Intake_actuator::Output);
std::ostream& operator<<(std::ostream&,Intake_actuator::Status_detail);
std::ostream& operator<<(std::ostream&,Intake_actuator const&);

bool operator<(Intake_actuator::Input,Intake_actuator::Input);
bool operator==(Intake_actuator::Input,Intake_actuator::Input);
bool operator!=(Intake_actuator::Input,Intake_actuator::Input);

bool operator==(Intake_actuator::Estimator,Intake_actuator::Estimator);
bool operator!=(Intake_actuator::Estimator,Intake_actuator::Estimator);

bool operator==(Intake_actuator,Intake_actuator);
bool operator!=(Intake_actuator,Intake_actuator);

Intake_actuator::Output control(Intake_actuator::Status,Intake_actuator::Goal);
Intake_actuator::Status status(Intake_actuator::Status);
bool ready(Intake_actuator::Status,Intake_actuator::Goal);

#endif
