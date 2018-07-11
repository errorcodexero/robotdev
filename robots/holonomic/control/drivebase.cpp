#include "drivebase.h"
#include <iostream>
#include <math.h>
#include "util.h"
#include "robot_constants.h"
#include "motion_profile.h"
#include "message_logger.h"
#include "point.h"
#include <fstream>

using namespace std;

#define FL_MOTOR_LOC 1
#define FR_MOTOR_LOC 2
#define B_MOTOR_LOC  0

DrivebaseController Drivebase::drivebase_controller;

unsigned pdb_location(Drivebase::Motor m){
	#define X(NAME,INDEX) if(m==Drivebase::Motor::NAME) return INDEX;
	X(FRONTLEFT,1)
	X(FRONTRIGHT,0)
	X(BACK,2)
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
	Drivebase::Distances d = {0.0,0.0,0.0};
	#define X(TYPE,SIDE) d.SIDE = ticks_to_inches(ticks.SIDE);
	DISTANCES_ITEMS(X)
	#undef X
	return d;
}

int inches_to_ticks(const double inches){
	return (int)(inches/(INCHES_PER_TICK));
}

/*
#define FL_ENCODER_PORTS 0,1
#define FR_ENCODER_PORTS 2,3
#define B_ENCODER_PORTS  3,4
#define FL_ENCODER_LOC 0
#define FR_ENCODER_LOC 1
#define B_ENCODER_LOC  2
*/

Robot_inputs Drivebase::Input_reader::operator()(Robot_inputs all,Input in)const{
	for(unsigned i=0;i<MOTORS;i++){
		all.current[pdb_location((Motor)i)]=in.current[i];
	}
	
	/*
	auto set=[&](unsigned index,Digital_in value){
		all.digital_io.in[index]=value;
	};
	auto encoder=[&](unsigned a,unsigned b,Encoder_info e){
		set(a,e.first);
		set(b,e.second);
	};
	encoder(FL_ENCODER_PORTS,in.fl);
	encoder(FR_ENCODER_PORTS,in.fr);
	encoder(B_ENCODER_PORTS,in.b);
	all.digital_io.encoder[FL_ENCODER_LOC] = inches_to_ticks(in.distances.fl);
	all.digital_io.encoder[FR_ENCODER_LOC] = inches_to_ticks(in.distances.fr);
	all.digital_io.encoder[B_ENCODER_LOC] = inches_to_ticks(in.distances.b);
	*/
	return all;
}

Drivebase::Input Drivebase::Input_reader::operator()(Robot_inputs const& in)const{
	/*auto encoder_info=[&](unsigned a, unsigned b){
		return make_pair(in.digital_io.in[a],in.digital_io.in[b]);
	};*/
	return Drivebase::Input{
		[&](){
			array<double,Drivebase::MOTORS> r;
			for(unsigned i=0;i<Drivebase::MOTORS;i++){
				Drivebase::Motor m=(Drivebase::Motor)i;
				r[i]=in.current[pdb_location(m)];
			}
			return r;
		}()
		/*encoder_info(FL_ENCODER_PORTS),
		encoder_info(FR_ENCODER_PORTS),
		encoder_info(B_ENCODER_PORTS),
		{
			ticks_to_inches(encoderconv(in.digital_io.encoder[FL_ENCODER_LOC])),
			ticks_to_inches(encoderconv(in.digital_io.encoder[FR_ENCODER_LOC])),
			ticks_to_inches(encoderconv(in.digital_io.encoder[B_ENCODER_LOC]))
		}*/
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

Drivebase::Encoder_ticks::Encoder_ticks():fl(0),fr(0),b(0){}
Drivebase::Distances::Distances():fl(0),fr(0),b(0){}
Drivebase::Distances::Distances(double d):fl(d),fr(d),b(d){}

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
			Motor_check::Status::OK_,
			Motor_check::Status::OK_
		},
		false,
		{0.0, 0.0, 0.0},
		{0, 0, 0},
		*examples((Drivebase::Output*)nullptr).begin(),
		0.0,
		0.0
	}};
}

set<Drivebase::Goal> examples(Drivebase::Goal*){
	return {Drivebase::Goal{0.0, 0.0, 0.0}};
}

ostream& operator<<(ostream& o,Drivebase::Goal const& a){
	o<<"Drivebase::Goal(";
	o<<"x: "<<a.x;
	o<<" y: "<<a.y;
	o<<" theta: "<<a.theta;
	o<<")";
	return o;
}

bool operator==(Drivebase::Goal const& a,Drivebase::Goal const& b){
	return a.x==b.x && a.y==b.y && a.theta==b.theta;
}

#define CMP(name) if(a.name<b.name) return 1; if(b.name<a.name) return 0;

bool operator<(Drivebase::Goal const& a,Drivebase::Goal const& b){
	CMP(x)
	CMP(y)
	CMP(theta)
	return false;
}

CMP_OPS(Drivebase::Output,DRIVEBASE_OUTPUT)

set<Drivebase::Output> examples(Drivebase::Output*){
	return {
		Drivebase::Output{0, 0, 0},
		Drivebase::Output{1, 1, 1}
	};
}

set<Drivebase::Input> examples(Drivebase::Input*){
	//auto d=Digital_in::_0;
	//auto p=make_pair(d,d);
	return {Drivebase::Input{
		{0,0,0}//,p,p,p,{0,0,0}
	}};
}

Drivebase::Estimator::Estimator():motor_check(),last({{{}},false,{0,0,0},{0,0,0},{0,0,0},0.0,0.0}){}

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
	X(FRONTLEFT,fl)
	X(FRONTRIGHT,fr)
	X(BACK,b)
	assert(0);
}

void Drivebase::Estimator::update(Time now,Drivebase::Input in,Drivebase::Output out){
	last.dt = now - last.now;//TODO: should now come from input?
	last.now = now;
	last.last_output = out;

	/*
	speed_timer.update(now,true);
	static const double POLL_TIME = .05;//seconds
	if(speed_timer.done()){
		last.speeds.fl = (last.distances.fl-in.distances.fl)/POLL_TIME;
		last.speeds.fr = (last.distances.fr-in.distances.fr)/POLL_TIME;
		last.speeds.b = (last.distances.b-in.distances.b)/POLL_TIME;
		speed_timer.set(POLL_TIME);
	}
	
	last.distances = in.distances;
	*/

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
	
	double mean_speed = (fabs(last.speeds.fl) + fabs(last.speeds.fr) + fabs(last.speeds.b)) / 3.0;
	stall_monitor.update(mean(in.current),mean_speed);
	if(stall_monitor.get()) last.stall = *(stall_monitor.get());
	*/
}

Robot_outputs Drivebase::Output_applicator::operator()(Robot_outputs robot,Drivebase::Output b)const{
	robot.pwm[FL_MOTOR_LOC] = b.fl;
	robot.pwm[FR_MOTOR_LOC] = b.fr;
	robot.pwm[B_MOTOR_LOC] = b.b;

	/*
	auto set_encoder=[&](unsigned int a, unsigned int b,unsigned int loc){
		robot.digital_io[a] = Digital_out::encoder(loc,1);
		robot.digital_io[b] = Digital_out::encoder(loc,0);
	};
	
	set_encoder(FL_ENCODER_PORTS,FL_ENCODER_LOC);
	set_encoder(FR_ENCODER_PORTS,FR_ENCODER_LOC);
	set_encoder(B_ENCODER_PORTS,B_ENCODER_LOC);
	*/

	return robot;
}

Drivebase::Output Drivebase::Output_applicator::operator()(Robot_outputs robot)const{
	return Drivebase::Output{	
		robot.pwm[FL_MOTOR_LOC],
		robot.pwm[FR_MOTOR_LOC],
		robot.pwm[B_MOTOR_LOC]
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

Drivebase::Output old_conversion(Drivebase::Goal goal) {
	Drivebase::Output r{0,0,0};

	r.fl=-double(1)/3* goal.theta- double(1)/3* goal.x -(double(1)/sqrt(3))*goal.y;
	r.fr=-double(1)/3* goal.theta- double(1)/3* goal.x +(double(1)/sqrt(3))*goal.y;
	r.b=(-(double(1)/3)* goal.theta) + ((double(2)/3)* goal.x);

	const double s=sqrt(3);
    r.fl*=s;
    r.fr*=s;
    r.b*=s;

    const double m=max(max(fabs(r.fr),fabs(r.fl)),fabs(r.b));
    if(m>1){
        r.fl/=m;
        r.fr/=m;
        r.b/=m;
    }

    return r;
}

Drivebase::Output new_conversion(Drivebase::Goal goal) {
	//http://robocup.mi.fu-berlin.de/buch/omnidrive.pdf
	Drivebase::Output r{0, 0, 0};

	r.fr = -0.5*goal.x +  .866*goal.y + goal.theta;
	r.fl = -0.5*goal.x + -.866*goal.y + goal.theta;
	r.b = goal.x + goal.theta;

	double max_power = max(max(r.fr, r.fl), r.b);
	if(max_power > 1) {
		r.fr /= max_power;
		r.fl /= max_power;
		r.b /= max_power;
	}

	return r;
}

Drivebase::Output control(Drivebase::Status status,Drivebase::Goal goal){
	//return Drivebase::Output{goal.x, goal.y, goal.theta};
	if(goal.use_new)
		return new_conversion(goal);
	else
		return old_conversion(goal);
}

Drivebase::Status status(Drivebase::Status a){ return a; }

bool ready(Drivebase::Status status,Drivebase::Goal goal){
	return true;
}

#ifdef DRIVEBASE_TEST
#include "formal.h"
#include <unistd.h>
#include <fstream>

int main(){
	{
		Drivebase d;//TODO: re-enable
		tester(d);
	}
}
#endif
