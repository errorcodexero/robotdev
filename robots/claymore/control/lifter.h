#ifndef LIFTER_H
#define LIFTER_H

#include "../util/interface.h"

struct Lifter{
	struct Goal{
		public:
		#define LIFTER_GOAL_MODES X(GO_TO_HEIGHT) X(UP) X(DOWN) X(STOP)
		enum class Mode{
			#define X(MODE) MODE,
			LIFTER_GOAL_MODES
			#undef X
		};//TODO: add modes for swtich, scale, etc
		
		private:
		Goal();
		Mode mode_;
		double target_;
		double tolerance_;
		
		public:
		Mode mode()const;
		double target()const;
		double tolerance()const;

		static Goal go_to_height(double,double);
		static Goal up();
		static Goal down();
		static Goal stop();
	};

	using Output = double;//pwm value, positive is up

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
		double height;
	
		Status_detail();
		Status_detail(bool,bool,double);
	};

	#define LIFTER_STATUSES X(BOTTOM) X(MIDDLE) X(TOP) X(ERROR)
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

std::ostream& operator<<(std::ostream&, Lifter::Status const&);

bool operator==(Lifter::Estimator const&, Lifter::Estimator const&);
bool operator!=(Lifter::Estimator const&, Lifter::Estimator const&);

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
