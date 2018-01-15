#include "drivebase.h"
#include <iostream>
#include <math.h>
#include "../util/util.h"
#include "../util/robot_constants.h"
#include "../util/motion_profile.h"
//temp
#include "../util/point.h"
#include <fstream>
//end temp
using namespace std;

//these are all off by one
#define L_MOTOR_LOC_1 0
#define L_MOTOR_LOC_2 1
#define R_MOTOR_LOC_1 2
#define R_MOTOR_LOC_2 3

unsigned pdb_location(Drivebase::Motor m){
	#define X(NAME,INDEX) if(m==Drivebase::NAME) return INDEX;
	//WILL NEED CORRECT VALUES
	X(LEFT1,0)
	X(LEFT2,1)
	X(RIGHT1,2)
	X(RIGHT2,13)
	#undef X
	assert(0);
	//assert(m>=0 && m<Drivebase::MOTORS);
}

int encoderconv(Maybe_inline<Encoder_output> encoder){
	if(encoder) return *encoder;
	return 10000;
}

const unsigned int TICKS_PER_REVOLUTION=200;
const double WHEEL_CIRCUMFERENCE=Robot_constants::DRIVE_WHEEL_DIAMETER*PI;//inches
const double INCHES_PER_TICK=WHEEL_CIRCUMFERENCE/(double)TICKS_PER_REVOLUTION;

double ticks_to_inches(const int ticks){
	return ticks*INCHES_PER_TICK;
}

Drivebase::Distances ticks_to_inches(const Drivebase::Encoder_ticks ticks){
	Drivebase::Distances d = {0.0,0.0};
	#define X(TYPE,SIDE) d.SIDE = ticks_to_inches(ticks.SIDE);
	DISTANCES_ITEMS(X)
	#undef X
	return d;
}

int inches_to_ticks(const double inches){
	return (int)(inches/(INCHES_PER_TICK));
}

#define L_ENCODER_PORTS 0,1
#define R_ENCODER_PORTS 2,3
#define L_ENCODER_LOC 0
#define R_ENCODER_LOC 1

Robot_inputs Drivebase::Input_reader::operator()(Robot_inputs all,Input in)const{
	for(unsigned i=0;i<MOTORS;i++){
		all.current[pdb_location((Motor)i)]=in.current[i];
	}
	auto set=[&](unsigned index,Digital_in value){
		all.digital_io.in[index]=value;
	};
	auto encoder=[&](unsigned a,unsigned b,Encoder_info e){
		set(a,e.first);
		set(b,e.second);
	};
	encoder(L_ENCODER_PORTS,in.left);
	encoder(R_ENCODER_PORTS,in.right);
	all.digital_io.encoder[L_ENCODER_LOC] = -inches_to_ticks(in.distances.l);
	all.digital_io.encoder[R_ENCODER_LOC] = inches_to_ticks(in.distances.r);
	all.navx.angle = in.angle;
	return all;
}

Drivebase::Input Drivebase::Input_reader::operator()(Robot_inputs const& in)const{
	auto encoder_info=[&](unsigned a, unsigned b){
		return make_pair(in.digital_io.in[a],in.digital_io.in[b]);
	};
	//cout<<in.navx.angle<<" "<<total_angle_to_displacement(in.navx.angle)<<" "<<in.navx.yaw<<"\n";
	return Drivebase::Input{
		[&](){
			array<double,Drivebase::MOTORS> r;
			for(unsigned i=0;i<Drivebase::MOTORS;i++){
				Drivebase::Motor m=(Drivebase::Motor)i;
				r[i]=in.current[pdb_location(m)];
			}
			return r;
		}(),
		encoder_info(L_ENCODER_PORTS),
		encoder_info(R_ENCODER_PORTS),
		{
			-ticks_to_inches(encoderconv(in.digital_io.encoder[L_ENCODER_LOC])),
			ticks_to_inches(encoderconv(in.digital_io.encoder[R_ENCODER_LOC]))
		},
		in.navx.angle
	};
}

float range(const Robot_inputs in){
	float volts=in.analog[2];
	const float voltsperinch=1; 
	float inches=volts*voltsperinch;
	return inches;
}

Drivebase::Encoder_ticks operator+(Drivebase::Encoder_ticks const& a,Drivebase::Encoder_ticks const& b){
	Drivebase::Encoder_ticks sum = {
		#define X(TYPE,SIDE) 0,
		ENCODER_TICKS(X)
		#undef X
	};
	#define X(TYPE,SIDE) sum.SIDE = a.SIDE + b.SIDE;
	ENCODER_TICKS(X)
	#undef X
	return sum;
}


Drivebase::Distances operator+(Drivebase::Distances const& a,Drivebase::Distances const& b){
	Drivebase::Distances sum = {
		#define X(TYPE,SIDE) 0,
		DISTANCES_ITEMS(X)
		#undef X
	};
	#define X(TYPE,SIDE) sum.SIDE = a.SIDE + b.SIDE;
	DISTANCES_ITEMS(X)
	#undef X
	return sum;
}

Drivebase::Distances& operator+=(Drivebase::Distances& a,Drivebase::Distances const& b){
	#define X(TYPE,SIDE) a.SIDE += b.SIDE;
	DISTANCES_ITEMS(X)
	#undef X
	return a;
}

Drivebase::Encoder_ticks operator-(Drivebase::Encoder_ticks const& a){
	Drivebase::Encoder_ticks opposite = {
		#define X(TYPE,SIDE) -a.SIDE,
		ENCODER_TICKS(X)
		#undef X
	};
	return opposite;
}

Drivebase::Encoder_ticks operator-(Drivebase::Encoder_ticks const& a,Drivebase::Encoder_ticks const& b){
	return a + (-b);
}

bool operator==(Drivebase::Distances const& a,Drivebase::Distances const& b){
	#define X(TYPE,SIDE) if(a.SIDE != b.SIDE) return false;
	DISTANCES_ITEMS(X)
	#undef X
	return true;
}

bool operator!=(Drivebase::Distances const& a,Drivebase::Distances const& b){
	return !(a==b);
}

ostream& operator<<(ostream& o,Drivebase::Distances const& a){
	o<<"Distances(";
	#define X(TYPE,SIDE) o<</*""#SIDE<<":"<<*/a.SIDE<<" ";
	DISTANCES_ITEMS(X)
	#undef X
	return o<<")";
}

bool operator<(Drivebase::Distances const& a,Drivebase::Distances const& b){
	#define X(TYPE,SIDE) if(a.SIDE >= b.SIDE) return false;
	DISTANCES_ITEMS(X)
	#undef X
	return true;
}

Drivebase::Distances fabs(Drivebase::Distances const& a){
	Drivebase::Distances pos = {
		#define X(TYPE,SIDE) fabs(a.SIDE),
		DISTANCES_ITEMS(X)
		#undef X
	};
	return pos;
}


Drivebase::Distances operator-(Drivebase::Distances const& a){
	Drivebase::Distances opposite = {
		#define X(TYPE,SIDE) -a.SIDE,
		DISTANCES_ITEMS(X)
		#undef X
	};
	return opposite;
}


Drivebase::Distances operator-(Drivebase::Distances const& a,Drivebase::Distances const& b){
	return a + (-b);
}

Drivebase::Encoder_ticks::Encoder_ticks():l(0),r(0){}
Drivebase::Distances::Distances():l(0),r(0){}
Drivebase::Distances::Distances(double d):l(d),r(d){}

IMPL_STRUCT(Drivebase::Encoder_ticks::Encoder_ticks,ENCODER_TICKS)
IMPL_STRUCT(Drivebase::Speeds::Speeds,SPEEDS_ITEMS)
IMPL_STRUCT(Drivebase::Distances::Distances,DISTANCES_ITEMS)

IMPL_STRUCT(Drivebase::Status::Status,DRIVEBASE_STATUS)
IMPL_STRUCT(Drivebase::Input::Input,DRIVEBASE_INPUT)
IMPL_STRUCT(Drivebase::Output::Output,DRIVEBASE_OUTPUT)

CMP_OPS(Drivebase::Encoder_ticks,ENCODER_TICKS)
CMP_OPS(Drivebase::Speeds,SPEEDS_ITEMS)

CMP_OPS(Drivebase::Input,DRIVEBASE_INPUT)

CMP_OPS(Drivebase::Status,DRIVEBASE_STATUS)

set<Drivebase::Status> examples(Drivebase::Status*){
	return {Drivebase::Status{
		array<Motor_check::Status,Drivebase::MOTORS>{
			Motor_check::Status::OK_,
			Motor_check::Status::OK_
		}
		,
		false,
		{0.0,0.0},
		{0,0},
		*examples((Drivebase::Output*)nullptr).begin(),
		0.0,
		0.0,
		0.0,
		0.0
	}};
}

set<Drivebase::Goal> examples(Drivebase::Goal*){
	return {
		Drivebase::Goal::rotate(0),
		Drivebase::Goal::drive_straight(0,0,0),
		Drivebase::Goal::distances({0,0}),
		Drivebase::Goal::absolute(0,0),
		Drivebase::Goal::absolute(1,1)
	};
}

std::ostream& operator<<(std::ostream& o, Drivebase::Goal::Mode a){
	#define X(name) if(a==Drivebase::Goal::Mode::name) return o<<""#name;
	DRIVEBASE_GOAL_MODES
	#undef X
	nyi
}

Drivebase::Goal::Goal():mode_(Drivebase::Goal::Mode::ABSOLUTE),distances_({0,0}),angle_(0),left_(0),right_(0){}

Drivebase::Goal::Mode Drivebase::Goal::mode()const{
	return mode_;
}

Drivebase::Distances Drivebase::Goal::distances()const{
	assert(mode_ == Drivebase::Goal::Mode::DISTANCES || mode_ == Drivebase::Goal::Mode::DRIVE_STRAIGHT);
	return distances_;
}

double Drivebase::Goal::right()const{
	assert(mode_ == Drivebase::Goal::Mode::ABSOLUTE);
	return right_;
}

double Drivebase::Goal::left()const{
	assert(mode_ == Drivebase::Goal::Mode::ABSOLUTE);
	return left_;
}

double Drivebase::Goal::angle()const{
	assert(mode_ == Drivebase::Goal::Mode::ROTATE || mode_ == Drivebase::Goal::Mode::DRIVE_STRAIGHT);
	return angle_;
}

double Drivebase::Goal::angle_i()const{
	assert(mode_ == Drivebase::Goal::Mode::DRIVE_STRAIGHT);
	return angle_i_;
}

Drivebase::Goal Drivebase::Goal::distances(Drivebase::Distances distances){
	Drivebase::Goal a;
	a.mode_ = Drivebase::Goal::Mode::DISTANCES;
	a.distances_ = distances;
	return a;
}

Drivebase::Goal Drivebase::Goal::absolute(double left,double right){
	Drivebase::Goal a;
	a.mode_ = Drivebase::Goal::Mode::ABSOLUTE;
	a.left_ = left;
	a.right_ = right;
	return a;
}

Drivebase::Goal Drivebase::Goal::drive_straight(Drivebase::Distances target, double initial_angle, double initial_angle_i){
	Drivebase::Goal a;
	a.mode_ = Drivebase::Goal::Mode::DRIVE_STRAIGHT;
	a.distances_ = target;
	a.angle_ = initial_angle;
	a.angle_i_ = initial_angle_i;
	return a;
}

Drivebase::Goal Drivebase::Goal::rotate(double angle){
	Drivebase::Goal a;
	a.mode_ = Drivebase::Goal::Mode::ROTATE;
	a.angle_ = angle;
	return a;
}

ostream& operator<<(ostream& o,Drivebase::Goal const& a){
	o<<"Drivebase::Goal("<<a.mode()<<" ";
	switch(a.mode()){
		case Drivebase::Goal::Mode::ROTATE:
			o<<a.angle();
			break;
		case Drivebase::Goal::Mode::DRIVE_STRAIGHT:
			o<<a.distances()<<" "<<a.angle();
			break;
		case Drivebase::Goal::Mode::DISTANCES:
			o<<a.distances();
			break;
		case Drivebase::Goal::Mode::ABSOLUTE:
			o<<a.left()<<" "<<a.right();
			break;
		default: 
			nyi
	}
	o<<")";
	return o;
}


bool operator==(Drivebase::Goal const& a,Drivebase::Goal const& b){
	#define X(NAME) if(a.NAME != b.NAME) return false;
	X(mode())
	switch(a.mode()){
		case Drivebase::Goal::Mode::ABSOLUTE:
			X(left())
			X(right())
			break;
		case Drivebase::Goal::Mode::DISTANCES:
			X(distances())
			break;
		case Drivebase::Goal::Mode::DRIVE_STRAIGHT:
			X(distances())
			X(angle())
			break;
		case Drivebase::Goal::Mode::ROTATE:
			X(angle())
			break;
		default:
			nyi
	}
	#undef X
	return true;	
}


#define CMP(name) if(a.name<b.name) return 1; if(b.name<a.name) return 0;

bool operator<(Drivebase::Goal const& a,Drivebase::Goal const& b){
	CMP(mode())
	switch(a.mode()){
		case Drivebase::Goal::Mode::ABSOLUTE:
			CMP(left())
			CMP(right())
			break;
		case Drivebase::Goal::Mode::DISTANCES:
			CMP(distances())
			break;
		case Drivebase::Goal::Mode::DRIVE_STRAIGHT:
			CMP(distances())
			CMP(angle())
			break;
		case Drivebase::Goal::Mode::ROTATE:
			CMP(angle())
			break;
		default:
			nyi
	}
	return 0;
}

CMP_OPS(Drivebase::Output,DRIVEBASE_OUTPUT)

set<Drivebase::Output> examples(Drivebase::Output*){
	return {
		Drivebase::Output{0,0},
		Drivebase::Output{1,1}
	};
}

set<Drivebase::Input> examples(Drivebase::Input*){
	auto d=Digital_in::_0;
	auto p=make_pair(d,d);
	return {Drivebase::Input{
		{0,0,0,0},p,p,{0,0},0.0,0.0
	}};
}

Drivebase::Estimator::Estimator():motor_check(),last({{{}},false,{0,0},{0,0},{0,0},0.0,0.0,0.0,0.0}){}

Drivebase::Status_detail Drivebase::Estimator::get()const{
	/*array<Motor_check::Status,MOTORS> a;
	for(unsigned i=0;i<a.size();i++){
		a[i]=motor_check[i].get();
	}*/
	
	return last;//Status{a,stall,piston/*,speeds,last_ticks*/};
}

ostream& operator<<(ostream& o,Drivebase::Output_applicator){
	return o<<"output_applicator";
}

ostream& operator<<(ostream& o,Drivebase const& a){
	return o<<"Drivebase("<<a.estimator.get()<<")";
}

double get_output(Drivebase::Output out,Drivebase::Motor m){
	#define X(NAME,POSITION) if(m==Drivebase::NAME) return out.POSITION;
	X(LEFT1,l)
	X(LEFT2,l)
	X(RIGHT1,r)
	X(RIGHT2,r)
	#undef X
	assert(0);
}

void Drivebase::Estimator::update(Time now,Drivebase::Input in,Drivebase::Output out){
	last.dt = now - last.now;//TODO: should now come from input?
	last.now = now;
	last.last_output = out;
	last.prev_angle = last.angle;
	last.angle = in.angle;

	speed_timer.update(now,true);
	static const double POLL_TIME = .05;//seconds
	if(speed_timer.done()){
		last.speeds.l = (last.distances.l-in.distances.l)/POLL_TIME;
		last.speeds.r = (last.distances.r-in.distances.r)/POLL_TIME;
		speed_timer.set(POLL_TIME);
	}
	
	last.distances = in.distances;

	for(unsigned i=0;i<MOTORS;i++){
		Drivebase::Motor m=(Drivebase::Motor)i;
		auto current=in.current[i];
		auto set_power_level=get_output(out,m);
		motor_check[i].update(now,current,set_power_level);
	}
	/*
	static const double STALL_CURRENT = .30;//from testing with autonomous
	static const double STALL_SPEED = .10;//ft/s speed at which we assume robot is stalled when current spikes
	last.stall = mean(in.current) > STALL_CURRENT && mean(fabs(last.speeds.l),fabs(last.speeds.r)) < STALL_SPEED;
	cout<<"curr:"<<mean(in.current)<<" "<<mean(last.speeds.l,last.speeds.r)<<"\n";
	*/
	stall_monitor.update(mean(in.current),mean(fabs(last.speeds.l),fabs(last.speeds.r)));
	if(stall_monitor.get()) last.stall = *(stall_monitor.get());
}

Robot_outputs Drivebase::Output_applicator::operator()(Robot_outputs robot,Drivebase::Output b)const{
	robot.talon_srx[L_MOTOR_LOC_1].power_level = b.l;
	robot.talon_srx[L_MOTOR_LOC_2].power_level = b.l;
	robot.talon_srx[R_MOTOR_LOC_1].power_level = -b.r;
	robot.talon_srx[R_MOTOR_LOC_2].power_level = -b.r;//reverse right side for software dev bot 2017

	auto set_encoder=[&](unsigned int a, unsigned int b,unsigned int loc){
		robot.digital_io[a] = Digital_out::encoder(loc,1);
		robot.digital_io[b] = Digital_out::encoder(loc,0);
	};
	
	set_encoder(L_ENCODER_PORTS,L_ENCODER_LOC);
	set_encoder(R_ENCODER_PORTS,R_ENCODER_LOC);
	/*robot.digital_io[0]=Digital_out::encoder(0,1);
	robot.digital_io[1]=Digital_out::encoder(0,0);
	robot.digital_io[2]=Digital_out::encoder(1,1);
	robot.digital_io[3]=Digital_out::encoder(1,0);
	robot.digital_io[4]=Digital_out::encoder(2,1);
	robot.digital_io[5]=Digital_out::encoder(2,0);*/

	robot.digital_io[10] = Digital_out::one();

	return robot;
}

Drivebase::Output Drivebase::Output_applicator::operator()(Robot_outputs robot)const{
	//assuming both motors on the same side are set to the same value//FIXME ?
	return Drivebase::Output{	
		robot.talon_srx[L_MOTOR_LOC_1].power_level,
		-robot.talon_srx[R_MOTOR_LOC_1].power_level, //reverse right side for software dev bot 2017
	};
}

bool operator==(Drivebase::Output_applicator const&,Drivebase::Output_applicator const&){
	return true;
}

bool operator==(Drivebase::Estimator const& a,Drivebase::Estimator const& b){
	if(a.last != b.last) return false;
	if(a.speed_timer != b.speed_timer) return false;
	/*for(unsigned i=0; i<Drivebase::MOTORS; i++){
		if(a.motor_check[i]!=b.motor_check[i])return false;
	}*/
	return true;
}

bool operator!=(Drivebase::Estimator const& a,Drivebase::Estimator const& b){
	return !(a==b);
}

bool operator==(Drivebase const& a,Drivebase const& b){
	return a.estimator==b.estimator && a.output_applicator==b.output_applicator;
}

bool operator!=(Drivebase const& a,Drivebase const& b){
	return !(a==b);
}

//TODO: Rename units
Drivebase::Output trapezoidal_speed_control(Drivebase::Status status, Drivebase::Goal goal){
	Drivebase::Output out = {0,0};
	const double MAX_OUT = .6;//in "volts
	double avg_goal = (goal.distances().l + goal.distances().r) / 2;
	double avg_dist = (status.distances.l + status.distances.r) / 2;
	double avg_last = (status.last_output.l + status.last_output.r) / 2;
	{//for ramping up (based on time)
		const double SPEED_UP_TIME = 2000; //milliseconds
		const double SLOPE = MAX_OUT / SPEED_UP_TIME; //"volts"/ms //TODO: currently arbitrary value
		const double MAX_STEP = 0.2;//"volts" //TODO: currently arbitrary value
		const double MILLISECONDS_PER_SECOND = 1000 / 1;
		
		double step = clamp(status.dt * MILLISECONDS_PER_SECOND * SLOPE,-MAX_STEP,MAX_STEP);// in "volts" 
		//double l_step = copysign(step,goal.distances().l);
		//double r_step = copysign(step,goal.distances().r);
		
		//cout<<"\ndt:"<<status.dt * MILLISECONDS_PER_SECOND<<" ms step:"<<step<<" "<<status<<"\n";
		
		double avg_out = clamp(avg_last + step, -MAX_OUT, MAX_OUT);
		out = {avg_out, avg_out};
		//out = {clamp(status.last_output.l + l_step,-MAX_OUT,MAX_OUT),clamp(status.last_output.r + r_step,-MAX_OUT,MAX_OUT)};
	}	
	{//for ramping down (based on distance)
		//Drivebase::Distances error = goal.distances() - status.distances;
		double error = avg_goal - avg_dist;
		const double SLOW_WITHIN_DISTANCE = 60; //inches
		const double SLOPE = MAX_OUT / SLOW_WITHIN_DISTANCE; //"volts"/inches //TODO: currently arbitrary value
		
		if(error < SLOW_WITHIN_DISTANCE) {
			double slow_out = clamp(error * SLOPE, -avg_last, avg_last);
			out = {slow_out, slow_out};
		}
	
		/*
		if(error.l < SLOW_WITHIN_DISTANCE)
			out.l = clamp((error.l * SLOPE), -MAX_OUT, status.last_output.l);
	
		if(error.r < SLOW_WITHIN_DISTANCE)
			out.r = clamp((error.r * SLOPE), -MAX_OUT, status.last_output.r);
		*/
	}
	//const double P = .005;
	//double s_error = status.speeds.l - status.speeds.r;
	//out.l = clamp(out.l + P*s_error, -MAX_OUT, MAX_OUT);
	//out.r = clamp(out.r - P*s_error, -MAX_OUT, MAX_OUT);
	return out;
}

double total_angle_to_displacement(const double ANGLE){//converts total angle to a displacement
	static const int UNITS_PER_REV = 360;//degrees/rev
	double angle_displacement = fmod(ANGLE,UNITS_PER_REV);//relative dispalcement from starting angle, not total angle distance
	double signed_angle_displacement = (angle_displacement > (UNITS_PER_REV * 0.5)) ? ((UNITS_PER_REV * 0.5) - angle_displacement) : angle_displacement;//converts angle to the closest rotation, from 0-180 or 0-180
	return signed_angle_displacement;
}

Drivebase::Output rotation_control(Drivebase::Status status, Drivebase::Goal goal){
	Drivebase::Output out = {0,0};
	static const double MAX_OUT = 0.5;
	static const double P = 0.005;//TODO: currently arbitrary value
	
	double status_angle_displacement = total_angle_to_displacement(status.angle);
	double goal_angle_displacement = total_angle_to_displacement(goal.angle());
	
	double error = goal_angle_displacement - status_angle_displacement;
	double power = clamp(error*P,-MAX_OUT,MAX_OUT);
	out = Drivebase::Output(power,-power);

	static const double FLOOR = .15;
	if(fabs(out.l) > .0001 && fabs(out.l) < FLOOR) out.l = copysign(FLOOR, out.l);
	if(fabs(out.r) > .0001 && fabs(out.r) < FLOOR) out.r = copysign(FLOOR, out.r);

	cout<<status.now<<" / "<<goal.angle()<<" / "<<goal_angle_displacement<<" / "<<status.angle<<" / "<<status_angle_displacement<<" / "<<out.l<<":"<<out.r<<"\n";

	return out;
}

Drivebase::Output drive_straight(Drivebase::Status status, Drivebase::Goal goal){
	Drivebase::Output out = trapezoidal_speed_control(status,goal);

	static const double MAX_OUT = .6;
	static const double P = .05, I = .1, D = .005;
	double error = total_angle_to_displacement(goal.angle()) - total_angle_to_displacement(status.angle);
	double error_d = (error - (total_angle_to_displacement(goal.angle()) - total_angle_to_displacement(status.prev_angle))) / status.dt;
	double change = P*error + I*goal.angle_i() + D*error_d;
	out.l = clamp(out.l + change, -MAX_OUT, MAX_OUT);
	out.r = clamp(out.r - change, -MAX_OUT, MAX_OUT);

	static const double FLOOR = .08;
	if(fabs(out.l) > .0001 && fabs(out.l) < FLOOR) out.l = copysign(FLOOR, out.l);
	if(fabs(out.r) > .0001 && fabs(out.r) < FLOOR) out.r = copysign(FLOOR, out.r);

	//cout << status.now << " / " << status.distances.l << ":" << status.distances.r << " / " << out.l << ":" << out.r << " / " << status.angle << " / " << goal.angle() << " / " << error_d << " / " << goal.angle_i() << " / " << goal.distances().l << "\n";

	return out;
}

Drivebase::Output control(Drivebase::Status status,Drivebase::Goal goal){
	//cout << status.distances.l << " " << status.distances.r << "\n";
	switch(goal.mode()){
		case Drivebase::Goal::Mode::DISTANCES:
			return trapezoidal_speed_control(status,goal);
		case Drivebase::Goal::Mode::ABSOLUTE:
			return Drivebase::Output{goal.left(),goal.right()};
		case Drivebase::Goal::Mode::DRIVE_STRAIGHT:
			return drive_straight(status,goal);
		case Drivebase::Goal::Mode::ROTATE:
			return rotation_control(status,goal);
		default:
			nyi
	}
}

Drivebase::Status status(Drivebase::Status a){ return a; }

bool ready(Drivebase::Status status,Drivebase::Goal goal){
	switch(goal.mode()){
		case Drivebase::Goal::Mode::ABSOLUTE:
			return true;
		case Drivebase::Goal::Mode::DISTANCES:
			{
				const double TOLERANCE = 1;//inches
				return fabs(goal.distances().l - status.distances.l) < TOLERANCE;//TODO: this is just a placeholder for now
			}
		case Drivebase::Goal::Mode::DRIVE_STRAIGHT:
			{
				const double TOLERANCE = 1;//inches
				double left_error = fabs(goal.distances().l - status.distances.l),
					right_error = fabs(goal.distances().r - status.distances.r);
				return ((left_error + right_error) / 2) < TOLERANCE;
			}
		case Drivebase::Goal::Mode::ROTATE:
			{
				const double TOLERANCE = .2;//degrees
				double error = total_angle_to_displacement(goal.angle()) - total_angle_to_displacement(status.angle);
				//cout<<"error: "<<fabs(error)<<"   "<<(fabs(error) < TOLERANCE)<<"\n";
				return fabs(error) < TOLERANCE;
			}
		default:
			nyi
	}
}

#ifdef DRIVEBASE_TEST
#include "formal.h"
#include <unistd.h>
#include <fstream>

//temp
using Inch = double;

static const Inch ROBOT_WIDTH = 28; //inches, ignores bumpers //TODO: finds some way of dealing with constants like this and wheel diameter

struct Drivebase_sim{
	using Input=Drivebase::Input;
	using Output=Drivebase::Output;
	
	Point position; //x,y are in distance in feet, theta is in radians (positive is counterclockwise from straight forward)
	Time last_time;
	Drivebase::Distances distances;

	void update(Time t,bool enable,Output out){
		static const double POWER_TO_SPEED = 6.5 * 12;//speed is in/s assuming low gear
		Time dt=t-last_time;
		last_time=t;
		if(!enable) return;
		Drivebase::Speeds speeds = {out.l * POWER_TO_SPEED, out.r * POWER_TO_SPEED};
		Drivebase::Distances ddistances = {speeds.l * dt, speeds.r * dt};
		double avg_dist_traveled = mean(ddistances.l,ddistances.r);
		double dtheta = ((out.l-out.r)*POWER_TO_SPEED*dt)/ROBOT_WIDTH;
		/*
			Angle is calculated as the difference between the two sides' powers divided by 2 --   (out.l - out.r) / 2
			That is then converted to a distance   --   ((out.l - out.r) / 2) * POWER_TO_SPEED * dt
			That distance is then converted to an angle -- ((((out.l - our.r) / 2) * POWER_TO_SPEED * dt) * 2) / ROBOT_WIDTH
		*/
		double dy = avg_dist_traveled * cosf(position.theta);
		double dx = avg_dist_traveled * sinf(position.theta);
		distances += ddistances;
		position += {dx,dy,dtheta};
	}
	Input get()const{
		auto d = Digital_in::_0;
		auto p = make_pair(d,d);
		Drivebase::Input in = {Drivebase::Input{
			{0,0,0,0},p,p,distances,0.0
		}};
		return in;
	}

	Drivebase_sim():position({}),last_time(0),distances({0,0}){}

};

ostream& operator<<(ostream& o,Drivebase_sim const& a){
	return o << "Drivebase_sim(" << a.position << ")";
}

///end temp

int main(){
	{
		Drivebase d;//TODO: re-enable
		tester(d);
	}
	/*
	{
		Drivebase::Encoder_ticks a = {100,100}, b = {10,10};
		Drivebase::Encoder_ticks diff = a - b, sum = a + b, opp = -a;
		cout<<"\na:"<<a<<" b:"<<b<<" diff:"<<diff<<" sum:"<<sum<<" opp:"<<opp<<"\n";
	}
	*/
	{
		cout<<"\n==========================================================\n";
		Drivebase_sim drive_sim;
		Drivebase drive;
		const Time TIMESTEP = .020;//sec
		const Time MAX_TEST_LENGTH = 15;//sec
		
		Drivebase::Goal goal = Drivebase::Goal::distances({100,100});//inches
		
		const bool ENABLED = true;
		
		for(Time t = 0; t < MAX_TEST_LENGTH; t += TIMESTEP){
			Drivebase::Status_detail status = drive.estimator.get();
			Drivebase::Output out = control(status,goal);
			
			drive_sim.update(t,ENABLED,out);
			
			Drivebase::Input input = drive_sim.get();
			
			drive.estimator.update(t,input,out);
			
			cout<<"t:"<<t<<"\tgoal:"<<goal<<"\tstatus:"<<status<<"\n";
			
			if(ready(status,goal)){
				cout<<"t:"<<t<<"\tgoal "<<goal<<" reached with status "<<status<<".\nFinishing\n";
				break;
			}
		}
	}
}
#endif
