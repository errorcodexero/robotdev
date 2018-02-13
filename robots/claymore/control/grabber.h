#ifndef	GRABBER_H
#define GRABBER_H

#include "../util/interface.h"
#include <set>
#include "../util/countdown_timer.h"
#include "nop.h"
#include "grabber_controller.h"

struct Grabber{
	static GrabberController grabber_controller;

	struct Goal{
		public:
		#define GRABBER_GOAL_MODES X(GO_TO_ANGLE) X(GO_TO_PRESET) X(OPEN) X(CLOSE) X(STOP)
		enum class Mode{
			#define X(MODE) MODE,
			GRABBER_GOAL_MODES
			#undef X
		};
		
		private:
		Goal();
		Mode mode_;
		double target_;
		GrabberController::Preset preset_target_;
		
		public:
		Mode mode()const;
		double target()const;
		GrabberController::Preset preset_target()const;

		static Goal go_to_angle(double);
		static Goal go_to_preset(GrabberController::Preset);
		static Goal open();
		static Goal close();
		static Goal stop();
	};

	using Output = double;
	
	struct Input{
		int ticks;
		bool has_cube;
		
		Input();
		Input(int);
	};

	struct Status_detail{
		bool has_cube;
		double angle;
		double time, dt;
		
		Status_detail();
		Status_detail(double);
	};
	
	using Status = Status_detail;
	
	struct Input_reader{
		Grabber::Input operator()(Robot_inputs const&)const;
		Robot_inputs operator()(Robot_inputs,Grabber::Input)const;
	};

	struct Output_applicator{
		Robot_outputs operator()(Robot_outputs,Grabber::Output)const;
		Grabber::Output operator()(Robot_outputs const&)const;
	};

	struct Estimator{
		Status_detail last;

		void update(Time,Grabber::Input,Grabber::Output);
		Status_detail get()const;
		Estimator();
	};

	Input_reader input_reader;
	Output_applicator output_applicator;
	Estimator estimator;
};

std::set<Grabber::Goal> examples(Grabber::Goal*);
std::set<Grabber::Input> examples(Grabber::Input*);
std::set<Grabber::Output> examples(Grabber::Output*);
std::set<Grabber::Status_detail> examples(Grabber::Status_detail*);

std::ostream& operator<<(std::ostream&,Grabber::Goal);
std::ostream& operator<<(std::ostream&,Grabber::Input);
std::ostream& operator<<(std::ostream&,Grabber::Status_detail);
std::ostream& operator<<(std::ostream&,Grabber const&);

bool operator<(Grabber::Goal,Grabber::Goal);

bool operator<(Grabber::Status_detail,Grabber::Status_detail);
bool operator==(Grabber::Status_detail,Grabber::Status_detail);
bool operator!=(Grabber::Status_detail,Grabber::Status_detail);

bool operator<(Grabber::Input,Grabber::Input);
bool operator==(Grabber::Input,Grabber::Input);
bool operator!=(Grabber::Input,Grabber::Input);

bool operator==(Grabber::Estimator,Grabber::Estimator);
bool operator!=(Grabber::Estimator,Grabber::Estimator);

bool operator==(Grabber,Grabber);
bool operator!=(Grabber,Grabber);

Grabber::Output control(Grabber::Status,Grabber::Goal);
Grabber::Status status(Grabber::Status);
bool ready(Grabber::Status,Grabber::Goal);

#endif
