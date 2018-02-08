#ifndef	INTAKE_GRABBER_H
#define INTAKE_GRABBER_H

#include "../util/interface.h"
#include <set>
#include "../util/countdown_timer.h"
#include "nop.h"

struct Intake_grabber{
	struct Goal{
		public:
		#define INTAKE_GRABBER_GOAL_MODES X(GO_TO_ANGLE) X(OPEN) X(CLOSE) X(STOP)
		enum class Mode{
			#define X(MODE) MODE,
			INTAKE_GRABBER_GOAL_MODES
			#undef X
		};
		
		private:
		Goal();
		Mode mode_;
		double target_;
		double tolerance_;
		
		public:
		Mode mode()const;
		double target()const;
		double tolerance()const;

		static Goal go_to_angle(double,double);
		static Goal open();
		static Goal close();
		static Goal stop();
	};

	using Output = double;
	
	struct Input{
		int ticks_l;
		int ticks_r;
		
		Input();
		Input(int,int);
	};

	struct Status_detail{
		double angle_l;
		double angle_r;
		
		Status_detail();
		Status_detail(double,double);
	};
	
	using Status = Status_detail;
	
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

		void update(Time,Input,Output);
		Status_detail get()const;
		Estimator();
	};

	Input_reader input_reader;
	Output_applicator output_applicator;
	Estimator estimator;
};

std::set<Intake_grabber::Goal> examples(Intake_grabber::Goal*);
std::set<Intake_grabber::Input> examples(Intake_grabber::Input*);
std::set<Intake_grabber::Output> examples(Intake_grabber::Output*);
std::set<Intake_grabber::Status_detail> examples(Intake_grabber::Status_detail*);

std::ostream& operator<<(std::ostream&,Intake_grabber::Goal);
std::ostream& operator<<(std::ostream&,Intake_grabber::Input);
std::ostream& operator<<(std::ostream&,Intake_grabber::Status_detail);
std::ostream& operator<<(std::ostream&,Intake_grabber const&);

bool operator<(Intake_grabber::Input,Intake_grabber::Input);
bool operator==(Intake_grabber::Input,Intake_grabber::Input);
bool operator!=(Intake_grabber::Input,Intake_grabber::Input);

bool operator==(Intake_grabber::Estimator,Intake_grabber::Estimator);
bool operator!=(Intake_grabber::Estimator,Intake_grabber::Estimator);

bool operator==(Intake_grabber,Intake_grabber);
bool operator!=(Intake_grabber,Intake_grabber);

Intake_grabber::Output control(Intake_grabber::Status,Intake_grabber::Goal);
Intake_grabber::Status status(Intake_grabber::Status);
bool ready(Intake_grabber::Status,Intake_grabber::Goal);

#endif
