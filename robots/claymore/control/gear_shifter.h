#ifndef GEAR_SHIFTER_H
#define GEAR_SHIFTER_H

#include "nop.h"
#include "drivebase.h"
#include "../util/interface.h"
#include "../util/speed_tracker.h"
#include "../util/countdown_timer.h"

struct Gear_shifter{
	#define GEAR_SHIFTER_GOALS X(AUTO) X(LOW) X(HIGH)
	enum class Goal{
		#define X(A) A,
		GEAR_SHIFTER_GOALS
		#undef X
	};

	using Input=Drivebase::Input;
	using Input_reader=Drivebase::Input_reader;
	
	#define GEAR_SHIFTER_OUTPUTS X(LOW) X(HIGH)
	enum class Output{
		#define X(A) A,
		GEAR_SHIFTER_OUTPUTS
		#undef X
	};
	
	using Status=Nop::Status;
	using Status_detail=Output;

	struct Output_applicator{
		Robot_outputs operator()(Robot_outputs,Output)const;
		Output operator()(Robot_outputs)const;
	};

	#define GEAR_SHIFTER_ESTIMATOR(X) \
		X(Speed_tracker,l_tracker) \
		X(Speed_tracker,r_tracker) \
		X(double,last_current) \
		X(Output,last_output) \
		X(Output,recommended) \
		X(Countdown_timer,no_shift) \
		X(int, print_count)
	struct Estimator{
		GEAR_SHIFTER_ESTIMATOR(DECL1)
		
		Estimator();
		Status_detail get()const;
		void update(Time,Input,Output);
	};

	Input_reader input_reader;
	Output_applicator output_applicator;
	Estimator estimator;
};

CMP1(Gear_shifter::Estimator)

std::ostream& operator<<(std::ostream&,Gear_shifter::Goal const&);
std::ostream& operator<<(std::ostream&,Gear_shifter::Output const&);
std::ostream& operator<<(std::ostream&,Gear_shifter const&);

std::set<Gear_shifter::Goal> examples(Gear_shifter::Goal*);
std::set<Gear_shifter::Output> examples(Gear_shifter::Output*);

Gear_shifter::Status status(Gear_shifter::Status_detail);
Gear_shifter::Output control(Gear_shifter::Status_detail,Gear_shifter::Goal);
bool ready(Gear_shifter::Status,Gear_shifter::Goal);

#endif	
