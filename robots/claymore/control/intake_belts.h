#ifndef INTAKE_BELTS_H
#define INTAKE_BELTS_H

#include <iostream>
#include <set>
#include "../util/interface.h"
#include "nop.h"

struct Intake_belts{
	enum class Goal{IN,OFF,OUT};
	
	using Input=Nop::Input;
	
	typedef Input Status_detail;

	typedef Status_detail Status;

	using Input_reader=Nop::Input_reader;

	typedef Goal Output;
	
	struct Output_applicator{
		Robot_outputs operator()(Robot_outputs,Intake_belts::Output)const;
		Intake_belts::Output operator()(Robot_outputs)const;	
	};

	using Estimator=Nop::Estimator;

	Input_reader input_reader;
	Estimator estimator;
	Output_applicator output_applicator; 
};

std::ostream& operator<<(std::ostream&,Intake_belts::Goal);
std::ostream& operator<<(std::ostream&,Intake_belts);

bool operator<(Intake_belts::Status_detail,Intake_belts::Status_detail);
bool operator==(Intake_belts::Status_detail,Intake_belts::Status_detail);
bool operator!=(Intake_belts::Status_detail,Intake_belts::Status_detail);

bool operator==(Intake_belts::Output_applicator,Intake_belts::Output_applicator);

bool operator==(Intake_belts,Intake_belts);
bool operator!=(Intake_belts,Intake_belts);

std::set<Intake_belts::Goal> examples(Intake_belts::Goal*);
std::set<Intake_belts::Status_detail> examples(Intake_belts::Status_detail*);

Intake_belts::Output control(Intake_belts::Status_detail, Intake_belts::Goal);
Intake_belts::Status status(Intake_belts::Status_detail);
bool ready(Intake_belts::Status, Intake_belts::Goal);

#endif
