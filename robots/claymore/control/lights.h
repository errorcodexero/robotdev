#ifndef LIGHTS_H
#define LIGHTS_H

#include <iostream>
#include <set>
#include "../util/interface.h"
#include "nop.h"
#include "../util/pwm_transcriber.h"

struct Lights{
	static PWM_transcriber blinky_light_transcriber;
	static void init_blinky_light_transcriber();

	enum class Camera_light{ON,OFF};
	struct Goal{//TODO: use macros
		Camera_light camera_light;
	
		Goal();
		Goal(Camera_light);
	};
	
	using Input=Nop::Input;

	struct Status_detail{
		unsigned lifter_height;
		bool climbing;
		bool autonomous;//communicate mode?
		Alliance alliance;
		
		Time now;

		Status_detail();
		Status_detail(unsigned,bool,bool,Alliance,Time);
	};

	using Status = Status_detail;

	using Input_reader=Nop::Input_reader;

	struct Output{ 
		bool camera_light;
		double blinky_light_info;

		Output();
		Output(bool,double);
	};
	
	struct Output_applicator{
		Output operator()(Robot_outputs)const;	
		Robot_outputs operator()(Robot_outputs,Output)const;
	};

	struct Estimator{
		Status_detail last;
		
		void update(Time,Input,Output);
		Status_detail get()const;

		Estimator();
	};

	Input_reader input_reader;
	Estimator estimator;
	Output_applicator output_applicator; 
};

std::ostream& operator<<(std::ostream&,Lights::Camera_light);
std::ostream& operator<<(std::ostream&,Lights::Status_detail);
std::ostream& operator<<(std::ostream&,Lights::Output);
std::ostream& operator<<(std::ostream&,Lights::Goal);
std::ostream& operator<<(std::ostream&,Lights);

bool operator==(Lights::Status_detail,Lights::Status_detail);
bool operator!=(Lights::Status_detail,Lights::Status_detail);
bool operator<(Lights::Status_detail,Lights::Status_detail);

bool operator==(Lights::Output,Lights::Output);
bool operator!=(Lights::Output,Lights::Output);
bool operator<(Lights::Output,Lights::Output);

bool operator<(Lights::Goal,Lights::Goal);
bool operator==(Lights::Goal,Lights::Goal);
bool operator!=(Lights::Goal,Lights::Goal);

bool operator==(Lights::Output_applicator,Lights::Output_applicator);

bool operator==(Lights::Estimator,Lights::Estimator);
bool operator!=(Lights::Estimator,Lights::Estimator);

bool operator==(Lights,Lights);
bool operator!=(Lights,Lights);

std::set<Lights::Status_detail> examples(Lights::Status_detail*);
std::set<Lights::Output> examples(Lights::Output*);
std::set<Lights::Goal> examples(Lights::Goal*);

Lights::Output control(Lights::Status_detail, Lights::Goal);
bool ready(Lights::Status, Lights::Goal);
Lights::Status status(Lights::Status_detail const&);
#endif
