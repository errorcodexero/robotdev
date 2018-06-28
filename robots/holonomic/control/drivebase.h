#ifndef DRIVEBASE_H
#define DRIVEBASE_H

#include <iosfwd>
#include <set>
#include "interface.h"
#include "motor_check.h"
#include "drivebase_controller.h"
#include "quick.h"
#include "countdown_timer.h"
#include "stall_monitor.h"

struct Drivebase{
    static DrivebaseController drivebase_controller;

	enum Motor { FRONTLEFT, FRONTRIGHT, BACK, MOTORS };

    typedef std::pair<Digital_in,Digital_in> Encoder_info;

#define ENCODER_TICKS(X)			\
    X(double,fl)					\
    X(double,fr)                    \
	X(double,b)
    struct Encoder_ticks{
		STRUCT_MEMBERS(ENCODER_TICKS)
			
		Encoder_ticks();
		Encoder_ticks(ENCODER_TICKS(TYPES) bool=0);
    };

#define DISTANCES_ITEMS(X)			\
    X(double,fl)					\
    X(double,fr)                    \
	X(double,b)
    struct Distances{
		STRUCT_MEMBERS(DISTANCES_ITEMS)

		Distances();
		Distances(double);
		Distances(DISTANCES_ITEMS(TYPES) bool=0);
    };

#define DRIVEBASE_INPUT(X)				\
    X(SINGLE_ARG(std::array<double,MOTORS>),current)
    /*
	X(Encoder_info,fl)				\
    X(Encoder_info,fr)				\
	X(Encoder_info,b)               \
    X(Distances,distances)
	*/
    DECLARE_STRUCT(Input,DRIVEBASE_INPUT)

    struct Input_reader{
		Input operator()(Robot_inputs const&)const;
		Robot_inputs operator()(Robot_inputs,Input)const;
    };
    Input_reader input_reader;

#define DRIVEBASE_OUTPUT(X)			\
    X(double,fl)					\
    X(double,fr)					\
	X(double,b)
    DECLARE_STRUCT(Output,DRIVEBASE_OUTPUT)

#define SPEEDS_ITEMS(X)				\
    X(double,fl)					\
    X(double,fr)                    \
	X(double,b)
    DECLARE_STRUCT(Speeds,SPEEDS_ITEMS) //consider renaming to Velocities

#define DRIVEBASE_STATUS(X)					\
    X(SINGLE_ARG(std::array<Motor_check::Status,MOTORS>),motor)	\
    X(bool,stall)						\
    X(Speeds,speeds)						\
    X(Distances,distances)					\
    X(Output,last_output)					\
    X(Time,dt)							\
    X(Time,now)
    DECLARE_STRUCT(Status,DRIVEBASE_STATUS) //time is all in seconds

    typedef Status Status_detail;

    struct Estimator{
		std::array<Motor_check,MOTORS> motor_check;
		Status_detail last;
		Countdown_timer speed_timer;
		Stall_monitor stall_monitor;

		void update(Time,Input,Output);//TODO: update
		Status_detail get()const;//TODO: may need updating
		Estimator();
    };
    Estimator estimator;

    struct Output_applicator{
		Robot_outputs operator()(Robot_outputs,Output)const;
		Output operator()(Robot_outputs)const;
    };
    Output_applicator output_applicator;

    struct Goal{
		double x, y, theta;
    };
};

bool operator==(Drivebase::Encoder_ticks const&,Drivebase::Encoder_ticks const&);
bool operator!=(Drivebase::Encoder_ticks const&,Drivebase::Encoder_ticks const&);
bool operator<(Drivebase::Encoder_ticks const&,Drivebase::Encoder_ticks const&);
Drivebase::Encoder_ticks operator+(Drivebase::Encoder_ticks const&,Drivebase::Encoder_ticks const&);
Drivebase::Encoder_ticks operator-(Drivebase::Encoder_ticks const&);
Drivebase::Encoder_ticks operator-(Drivebase::Encoder_ticks const&,Drivebase::Encoder_ticks const&);
std::ostream& operator<<(std::ostream&,Drivebase::Encoder_ticks const&);

bool operator==(Drivebase::Distances const&,Drivebase::Distances const&);
bool operator!=(Drivebase::Distances const&,Drivebase::Distances const&);
bool operator<(Drivebase::Distances const&,Drivebase::Distances const&);
std::ostream& operator<<(std::ostream&,Drivebase::Distances const&);
Drivebase::Distances operator+(Drivebase::Distances const&,Drivebase::Distances const&);
Drivebase::Distances& operator+=(Drivebase::Distances&,Drivebase::Distances const&);
Drivebase::Distances operator-(Drivebase::Distances const&);
Drivebase::Distances operator-(Drivebase::Distances const&,Drivebase::Distances const&);
Drivebase::Distances fabs(Drivebase::Distances const&);

double ticks_to_inches(const int);
int inches_to_ticks(const double);
Drivebase::Distances ticks_to_inches(const Drivebase::Encoder_ticks);
Drivebase::Encoder_ticks inches_to_ticks(const Drivebase::Distances);

int encoderconv(Maybe_inline<Encoder_output>);

double total_angle_to_displacement(double);

CMP1(Drivebase::Encoder_ticks)
CMP1(Drivebase::Speeds)

std::ostream& operator<<(std::ostream&,Drivebase::Input const&);
bool operator<(Drivebase::Input const&,Drivebase::Input const&);
bool operator==(Drivebase::Input const&,Drivebase::Input const&);
bool operator!=(Drivebase::Input const&,Drivebase::Input const&);
std::set<Drivebase::Input> examples(Drivebase::Input*);

std::ostream& operator<<(std::ostream&,Drivebase::Output const&);
bool operator<(Drivebase::Output const&,Drivebase::Output const&);
bool operator==(Drivebase::Output const&,Drivebase::Output const&);
bool operator!=(Drivebase::Output const&,Drivebase::Output const&);
std::set<Drivebase::Output> examples(Drivebase::Output*);

CMP1(Drivebase::Status)
std::set<Drivebase::Status> examples(Drivebase::Status*);

std::ostream& operator<<(std::ostream&,Drivebase::Goal const&);
std::set<Drivebase::Goal> examples(Drivebase::Goal*);
bool operator<(Drivebase::Goal const&,Drivebase::Goal const&);
bool operator==(Drivebase::Goal const&,Drivebase::Goal const&);

Drivebase::Status status(Drivebase::Status_detail);
Drivebase::Output control(Drivebase::Status_detail,Drivebase::Goal);
bool ready(Drivebase::Status,Drivebase::Goal);

bool operator==(Drivebase::Estimator const&,Drivebase::Estimator const&);
bool operator!=(Drivebase::Estimator const&,Drivebase::Estimator const&);

bool operator!=(Drivebase const&,Drivebase const&);
std::ostream& operator<<(std::ostream&,Drivebase const&);

float range(const Robot_inputs); //To be used for wall following NOT DONE 

#endif
