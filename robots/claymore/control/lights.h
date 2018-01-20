#ifndef LIGHTS_H
#define LIGHTS_H

#include <iostream>
#include <set>
#include "../util/interface.h"
#include "nop.h"

struct Lights{
	enum class Camera_light{ON,OFF};
	struct Goal{//TODO: use macros
		Camera_light camera_light;

		Goal();
		Goal(Camera_light);
	};
	
	using Input=Nop::Input;

	using Status_detail = Nop::Status_detail;

	typedef Status_detail Status;

	using Input_reader=Nop::Input_reader;

	using Output = Goal;
	
	struct Output_applicator{
		Output operator()(Robot_outputs)const;	
		Robot_outputs operator()(Robot_outputs,Output)const;
	};

	struct Estimator{
		void update(Time,Input,Output);
		Status_detail get()const;

		Estimator();
	};

	Input_reader input_reader;
	Estimator estimator;
	Output_applicator output_applicator; 
};

std::ostream& operator<<(std::ostream&,Lights::Camera_light);
std::ostream& operator<<(std::ostream&,Lights::Goal);
std::ostream& operator<<(std::ostream&,Lights);

bool operator<(Lights::Goal,Lights::Goal);
bool operator==(Lights::Goal,Lights::Goal);
bool operator!=(Lights::Goal,Lights::Goal);

bool operator==(Lights::Output_applicator,Lights::Output_applicator);

bool operator==(Lights::Estimator,Lights::Estimator);
bool operator!=(Lights::Estimator,Lights::Estimator);

bool operator==(Lights,Lights);
bool operator!=(Lights,Lights);

std::set<Lights::Goal> examples(Lights::Goal*);

Lights::Output control(Lights::Status_detail, Lights::Goal);
bool ready(Lights::Status, Lights::Goal);

#endif
