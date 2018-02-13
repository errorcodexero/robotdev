#ifndef LIFTER_H
#define LIFTER_H

#include "../util/interface.h"
#include "lifter_controller.h"

struct Lifter{
	static LifterController lifter_controller;

	struct Goal{
		public:
		#define LIFTER_GEARING_MODES X(LOW) X(HIGH)
		enum class Gearing{
			#define X(MODE) MODE,
			LIFTER_GEARING_MODES
			#undef X
		};
		
		#define LIFTER_GOAL_MODES X(CLIMB) X(GO_TO_HEIGHT) X(GO_TO_PRESET) X(UP) X(DOWN) X(STOP)
		enum class Mode{
			#define X(MODE) MODE,
			LIFTER_GOAL_MODES
			#undef X
		};//TODO: add modes for switch, scale, etc?
		
		private:
		Goal();
		Mode mode_;
		Gearing gearing_;
		double target_;
		double tolerance_;
		LifterController::Preset preset_target_;
		bool high_power_;
		
		public:
		Mode mode()const;
		Gearing gearing()const;
		double target()const;
		double tolerance()const;
		LifterController::Preset preset_target()const;
		bool high_power()const;

		static Goal climb();
		static Goal go_to_height(double);
		static Goal go_to_preset(LifterController::Preset);
		static Goal up(bool);
		static Goal down(bool);
		static Goal stop();
	};

	struct Output{
		double power;//pwm value, positive is up
		using Gearing = Lifter::Goal::Gearing;
		Gearing gearing;
		
		Output();
		Output(double,Gearing);
	};

	struct Input{
		bool bottom_hall_effect;
		bool climbed_hall_effect;
		bool top_hall_effect;
		int ticks;
	
		Input();
		Input(bool,bool,bool,int);
	};

	struct Status_detail{
		bool at_bottom;
		bool at_top;
		bool at_climbed_height;
		double height;
		double time, dt;
	
		Status_detail();
		Status_detail(bool,bool,bool,double);
	};

	#define LIFTER_STATUSES X(BOTTOM) X(MIDDLE) X(TOP) X(ERROR) X(CLIMBED)
	enum class Status{
		#define X(MODE) MODE,
		LIFTER_STATUSES
		#undef X
	};
					 
	struct Output_applicator{
		Robot_outputs operator()(Robot_outputs,Lifter::Output const&)const;
		Output operator()(Robot_outputs const&)const;
	};

	struct Input_reader{
		Robot_inputs operator()(Robot_inputs,Lifter::Input const&)const;
		Input operator()(Robot_inputs const&)const;
	};

	struct Estimator{
		Status_detail last;

		void update(Time const&,Input const&,Output const&);
		Status_detail get()const;
		
		Estimator();
		Estimator(Lifter::Status_detail);
	};

	Output_applicator output_applicator;
	Input_reader input_reader;
	Estimator estimator;
};

bool operator==(Lifter::Input const&, Lifter::Input const&);
bool operator!=(Lifter::Input const&, Lifter::Input const&);
bool operator<(Lifter::Input const&,Lifter::Input const&);
std::ostream& operator<<(std::ostream&, Lifter::Input const&);

bool operator==(Lifter::Status_detail const&, Lifter::Status_detail const&);
bool operator!=(Lifter::Status_detail const&, Lifter::Status_detail const&);
bool operator<(Lifter::Status_detail const&,Lifter::Status_detail const&);
std::ostream& operator<<(std::ostream&,Lifter::Status_detail const&);

bool operator==(Lifter::Output const&,Lifter::Output const&);
bool operator!=(Lifter::Output const&,Lifter::Output const&);
bool operator<(Lifter::Output const&,Lifter::Output const&);
std::ostream& operator<<(std::ostream&, Lifter::Output const&);

std::ostream& operator<<(std::ostream&, Lifter::Status const&);

bool operator==(Lifter::Estimator const&, Lifter::Estimator const&);
bool operator!=(Lifter::Estimator const&, Lifter::Estimator const&);

bool operator==(Lifter::Goal const&, Lifter::Goal const&);
bool operator!=(Lifter::Goal const&, Lifter::Goal const&);
bool operator<(Lifter::Goal const&,Lifter::Goal const&);
std::ostream& operator<<(std::ostream&,Lifter::Goal const&);

std::set<Lifter::Input> examples(Lifter::Input*);
std::set<Lifter::Output> examples(Lifter::Output*);
std::set<Lifter::Status_detail> examples(Lifter::Status_detail*);
std::set<Lifter::Status> examples(Lifter::Status*);
std::set<Lifter::Goal> examples(Lifter::Goal*);

Lifter::Output control(Lifter::Status_detail const&,Lifter::Goal const&);
Lifter::Status status(Lifter::Status_detail const&);
bool ready(Lifter::Status const&,Lifter::Goal const&);

#endif
