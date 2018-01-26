#ifndef LIFTER_H
#define LIFTER_H

#include "../util/interface.h"

struct Lifter{
	struct Goal{
		public:
		enum class Mode{GO_TO_HEIGHT,UP,DOWN,STOP};//TODO: add modes for swtich, scale, etc
		
		private:
		Goal();
		Mode mode_;
		double height;

		public:
		Mode mode()const;
		double height()const;

		static Goal go_to_height(double,double);
		static Goal up();
		static Goal down();
		static Goal stop();
	};

	struct Output{

	};

	struct Input{

	};

	struct Status_detail{

	};

	struct Status{

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
		void update(Time const&,Input const&,Output const&);
		Status_detail get()const;
	};

	Output_applicator output_applicator;
	Input_reader input_reader;
	Estimator estimator;
};

std::set<Lifter::Input> examples(Lifter::Input*);
std::set<Lifter::Output> examples(Lifter::Output*);
std::set<Lifter::Status_detail> examples(Lifter::Status_detail*);
std::set<Lifter::Goal> examples(Lifter::Goal*);

Lifter::Output control(Lifter::Status_detail const&,Lifter::Goal const&);
Lifter::Status status(Lifter::Status_detail const&);
bool ready(Lifter::Status_detail const&,Lifter::Goal const&);

#endif
