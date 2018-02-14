#include "lifter.h"
#include "util.h"

using namespace std;

#define LIFTER_ADDRESS_L 0
#define LIFTER_ADDRESS_R 1
#define LIFTER_SHIFTER_LOW 2
#define LIFTER_SHIFTER_HIGH 3

#define CLIMB_POWER .60 //TODO tune
#define MANUAL_LIFTER_LOW_POWER .60 //TODO tune
#define MANUAL_LIFTER_HIGH_POWER .80 //TODO tune
#define AUTO_LIFTER_POWER .60 //TODO tune

#define BOTTOM_HALL_EFFECT_ADDRESS 9
#define CLIMBED_HALL_EFFECT_ADDRESS 2 //TODO MXP DIO 0
#define TOP_HALL_EFFECT_ADDRESS 8
#define ENCODER_ADDRESS 4 //TODO

LifterController Lifter::lifter_controller;

int bottom_ticks = 0, top_ticks = 100;//arbitrary

ostream& operator<<(ostream& o, Lifter::Goal::Gearing a){
#define X(GEARING) if(a==Lifter::Goal::Gearing::GEARING) return o<<""#GEARING;
    LIFTER_GEARING_MODES
#undef X
	assert(0);
}

ostream& operator<<(ostream& o, Lifter::Goal::Mode a){
#define X(MODE) if(a==Lifter::Goal::Mode::MODE) return o<<""#MODE;
    LIFTER_GOAL_MODES
#undef X
	assert(0);
}

Lifter::Goal::Mode Lifter::Goal::mode()const{
    return mode_;
}

double Lifter::Goal::target()const{
    return target_;
}

double Lifter::Goal::tolerance()const{
    return tolerance_;
}

Lifter::Goal::Gearing Lifter::Goal::gearing()const{
    return gearing_;
}

LifterController::Preset Lifter::Goal::preset_target()const{
    return preset_target_;
}

bool Lifter::Goal::high_power()const{
    return high_power_;
}

Lifter::Goal::Goal():mode_(Lifter::Goal::Mode::STOP),target_(0.0),tolerance_(0.0),high_power_(false){}

Lifter::Goal Lifter::Goal::climb(){
    Lifter::Goal a;
    a.mode_ = Lifter::Goal::Mode::CLIMB;
    a.gearing_ = Lifter::Goal::Gearing::LOW;
    return a;
}

Lifter::Goal Lifter::Goal::up(bool high_power = false){
    Lifter::Goal a;
    a.mode_ = Lifter::Goal::Mode::UP;
    a.gearing_ = Lifter::Goal::Gearing::HIGH;
    a.high_power_ = high_power;
    return a;
}

Lifter::Goal Lifter::Goal::stop(){
    Lifter::Goal a;
    a.mode_ = Lifter::Goal::Mode::STOP;
    a.gearing_ = Lifter::Goal::Gearing::HIGH;
    return a;
}

Lifter::Goal Lifter::Goal::down(bool high_power = false){
    Lifter::Goal a;
    a.mode_ = Lifter::Goal::Mode::DOWN;
    a.gearing_ = Lifter::Goal::Gearing::HIGH;
    a.high_power_ = high_power;
    return a;
}

Lifter::Goal Lifter::Goal::go_to_height(double target){
    Lifter::Goal a;
    a.mode_ = Lifter::Goal::Mode::GO_TO_HEIGHT;
    a.gearing_ = Lifter::Goal::Gearing::HIGH;
    a.target_ = target;
    return a;
}

Lifter::Goal Lifter::Goal::go_to_preset(LifterController::Preset preset){
    Lifter::Goal a;
    a.mode_ = Lifter::Goal::Mode::GO_TO_PRESET;
    a.gearing_ = Lifter::Goal::Gearing::HIGH;
    a.preset_target_ = preset;
    return a;
}

Lifter::Output::Output(double p, Lifter::Output::Gearing g):power(p),gearing(g){}
Lifter::Output::Output():Output(0,Lifter::Output::Gearing::HIGH){}

Lifter::Input::Input(bool b, bool c, bool t, int e):bottom_hall_effect(b),climbed_hall_effect(c),top_hall_effect(t),ticks(e){}
Lifter::Input::Input():Input(false,false,false,0){}

Lifter::Status_detail::Status_detail(bool b,bool t,bool c,double h):at_bottom(b),at_top(t),at_climbed_height(c),height(h){}
Lifter::Status_detail::Status_detail():Status_detail(false,false,false,0.0){}

Lifter::Estimator::Estimator(Lifter::Status_detail s):last(s){}
Lifter::Estimator::Estimator():Estimator(Lifter::Status_detail{}){}

#define CMP(VAR)				\
    if(a.VAR < b.VAR) return true;		\
    if(b.VAR < a.VAR) return false; 	

bool operator==(Lifter::Output const& a,Lifter::Output const& b){
    return a.power == b.power && a.gearing == b.gearing;
}

bool operator!=(Lifter::Output const& a,Lifter::Output const& b){
    return !(a==b);
}
	
bool operator<(Lifter::Output const& a,Lifter::Output const& b){
    CMP(power)
	CMP(gearing)
	return false;
}

ostream& operator<<(ostream& o, Lifter::Output const& a){
    o<<"(";
    o<<"power:"<<a.power;
    o<<" gearing:"<<a.gearing;
    o<<")";
    return o;
}
	
bool operator==(Lifter::Input const& a, Lifter::Input const& b){
    return true;
}

bool operator!=(Lifter::Input const& a, Lifter::Input const& b){
    return !(a==b);
}

bool operator<(Lifter::Input const& a,Lifter::Input const& b){
    return false;
}

ostream& operator<<(ostream& o, Lifter::Input const&){
    return o;
}

ostream& operator<<(ostream& o, Lifter::Status const& a){
#define X(STATUS) if(a==Lifter::Status::STATUS) return o<<""#STATUS;
    LIFTER_STATUSES
#undef X
	assert(0);
}

bool operator==(Lifter::Estimator const& a, Lifter::Estimator const& b){
    return true;
}

bool operator!=(Lifter::Estimator const& a, Lifter::Estimator const& b){
    return !(a==b);
}

bool operator==(Lifter::Goal const& a, Lifter::Goal const& b){
    return a.mode() == b.mode() && a.gearing() == b.gearing() && a.target() == b.target() && a.tolerance() == b.tolerance() && a.preset_target() == b.preset_target();
}

bool operator!=(Lifter::Goal const& a, Lifter::Goal const& b){
    return !(a==b);
}

bool operator<(Lifter::Goal const& a,Lifter::Goal const& b){
    if(a.mode() == b.mode() && a.mode() == Lifter::Goal::Mode::GO_TO_HEIGHT){
	CMP(target())
	    CMP(tolerance())
	    }
    return a.mode() < b.mode();
}

ostream& operator<<(ostream& o, Lifter::Goal const& a){
    o<<"Goal( "<<a.mode();
    if(a.mode() == Lifter::Goal::Mode::GO_TO_HEIGHT){
	o<<" "<<a.target();
	o<<" "<<a.tolerance();
    }
    o<<")";
    return o;
}

bool operator==(Lifter::Status_detail const& a,Lifter::Status_detail const& b){
    return a.at_top == b.at_top && a.at_bottom == b.at_bottom && a.at_climbed_height == b.at_climbed_height && a.height == b.height;
}

bool operator!=(Lifter::Status_detail const& a,Lifter::Status_detail const& b){
    return !(a==b);
}

bool operator<(Lifter::Status_detail const& a,Lifter::Status_detail const& b){
    CMP(at_top)
	CMP(at_bottom)
	CMP(at_climbed_height)
	CMP(height)
	return false;
}

ostream& operator<<(ostream& o, Lifter::Status_detail const& a){
    o<<"(";
    o<<"top:"<<a.at_top;
    o<<" bottom:"<<a.at_bottom;
    o<<" climbed:"<<a.at_climbed_height;
    o<<" height:"<<a.height;
    o<<")";
    return o;
}

ostream& operator<<(ostream& o, Lifter const& a){
    return o<<"Lifter("<<a.estimator.get()<<")";
}

#undef CMP

Robot_outputs Lifter::Output_applicator::operator()(Robot_outputs r, Lifter::Output const& out)const{
    r.pwm[LIFTER_ADDRESS_L] = out.power;
    r.pwm[LIFTER_ADDRESS_R] = out.power;
    r.solenoid[LIFTER_SHIFTER_LOW] = out.gearing == Lifter::Output::Gearing::LOW;
    r.solenoid[LIFTER_SHIFTER_HIGH] = out.gearing != Lifter::Output::Gearing::LOW;
    return r;
};

Lifter::Output Lifter::Output_applicator::operator()(Robot_outputs const& r)const{
    return {
	r.pwm[LIFTER_ADDRESS_L], //assuming that left and right sides are set to the same value
	r.solenoid[LIFTER_SHIFTER_LOW] ? Lifter::Output::Gearing::LOW : Lifter::Output::Gearing::HIGH
	   };
}

Robot_inputs Lifter::Input_reader::operator()(Robot_inputs r, Lifter::Input const& in)const{
    r.digital_io.in[BOTTOM_HALL_EFFECT_ADDRESS] = in.bottom_hall_effect ? Digital_in::_0 : Digital_in::_1;
    r.digital_io.in[CLIMBED_HALL_EFFECT_ADDRESS] = in.climbed_hall_effect ? Digital_in::_0 : Digital_in::_1;
    r.digital_io.in[TOP_HALL_EFFECT_ADDRESS] = in.top_hall_effect ? Digital_in::_0 : Digital_in::_1;//active low now I guess
    r.digital_io.encoder[ENCODER_ADDRESS] = in.ticks;
    return r;
}

Lifter::Input Lifter::Input_reader::operator()(Robot_inputs const& r)const{
    return {
	r.digital_io.in[BOTTOM_HALL_EFFECT_ADDRESS] == Digital_in::_0,
	    r.digital_io.in[CLIMBED_HALL_EFFECT_ADDRESS] == Digital_in::_0,
	    r.digital_io.in[TOP_HALL_EFFECT_ADDRESS] == Digital_in::_0,
	    r.digital_io.encoder[ENCODER_ADDRESS]
	    };
}

void Lifter::Estimator::update(Time const& now, Lifter::Input const& in, Lifter::Output const&){
    last.at_bottom = in.bottom_hall_effect;
    last.at_top = in.top_hall_effect;

    if(last.at_top != last.at_bottom){ // != being used like xor
	if(last.at_top){
	    top_ticks = in.ticks;
	} 
	if(last.at_bottom){ 
	    bottom_ticks = in.ticks;
	}
    }
    const double MAX_LIFTER_HEIGHT = 100;//inches //TODO arbitrary
    const double INCHES_PER_TICK = MAX_LIFTER_HEIGHT / (top_ticks - bottom_ticks); 

    last.height = in.ticks*INCHES_PER_TICK;
	
    last.dt = last.time - now;
    last.time = now;
}

Lifter::Status_detail Lifter::Estimator::get()const{
    return last;
}

set<Lifter::Input> examples(Lifter::Input*){
    return {
	{false,false,false,0},
	{true,false,false,0},
	{true,true,false,0},
	{true,true,true,0},
	{false,true,false,0},
	{false,true,true,0},
	{false,false,true,0},
	{true,false,true,0}
    };
}

set<Lifter::Output> examples(Lifter::Output*){
    return {
	{-MANUAL_LIFTER_LOW_POWER, Lifter::Output::Gearing::HIGH},
	{0.0, Lifter::Output::Gearing::HIGH}, 
	{MANUAL_LIFTER_LOW_POWER, Lifter::Output::Gearing::HIGH},
	{0.0, Lifter::Output::Gearing::LOW},
	{CLIMB_POWER, Lifter::Output::Gearing::LOW},
	    };
}

set<Lifter::Status_detail> examples(Lifter::Status_detail*){
    return {
	{false,false,false,0.0},
	{false,false,true,0.0},
	{true,false,false,0.0},
	{true,false,true,0.0},
	{false,true,false,0.0},
	{false,true,true,0.0},
	{true,true,false,0.0},
	{true,true,true,0.0}
    };
}

set<Lifter::Status> examples(Lifter::Status*){
    return {Lifter::Status::CLIMBED,Lifter::Status::MIDDLE,Lifter::Status::BOTTOM,Lifter::Status::TOP,Lifter::Status::ERROR};
}

set<Lifter::Goal> examples(Lifter::Goal*){
    return {Lifter::Goal::climb(),Lifter::Goal::up(),Lifter::Goal::stop(),Lifter::Goal::down(),Lifter::Goal::go_to_height(0.0)};
}

Lifter::Output control(Lifter::Status_detail const& status_detail, Lifter::Goal const& goal){
    Lifter::Status s = status(status_detail);
    if(s == Lifter::Status::ERROR){
	return {0.0, goal.gearing()};
    }

    Lifter::Output out = {0.0, goal.gearing()};
    switch(goal.mode()){
    case Lifter::Goal::Mode::CLIMB:
	if(s != Lifter::Status::CLIMBED)
		out.power = -CLIMB_POWER;
	else
		out.power = 0.0;
	break;
    case Lifter::Goal::Mode::UP:
	if(s != Lifter::Status::TOP)
		out.power = goal.high_power() ? MANUAL_LIFTER_HIGH_POWER : MANUAL_LIFTER_LOW_POWER;
	else
		out.power = 0.0;
	break;
    case Lifter::Goal::Mode::DOWN:
	if(s != Lifter::Status::BOTTOM)
		out.power = goal.high_power() ? -MANUAL_LIFTER_HIGH_POWER : -MANUAL_LIFTER_LOW_POWER;
	else
		out.power = 0.0;
	break;
    case Lifter::Goal::Mode::STOP:
	Lifter::lifter_controller.idle(status_detail.height, status_detail.time, status_detail.dt);
	out.power = 0.0;
	break; 
    case Lifter::Goal::Mode::GO_TO_HEIGHT:
	Lifter::lifter_controller.updateHeightOnChange(goal.target(), status_detail.time);
	Lifter::lifter_controller.update(status_detail.height, status_detail.time, status_detail.dt, out.power);
	break;
    case Lifter::Goal::Mode::GO_TO_PRESET:
	Lifter::lifter_controller.updateHeightOnChange(goal.preset_target(), status_detail.time);
	Lifter::lifter_controller.update(status_detail.height, status_detail.time, status_detail.dt, out.power);
	break;
    default:
	nyi
	    }
    return out;
}

Lifter::Status status(Lifter::Status_detail const& status_detail){
    if(
	(status_detail.at_top && status_detail.at_bottom) ||
	(status_detail.at_top && status_detail.at_climbed_height) ||
	(status_detail.at_bottom && status_detail.at_climbed_height)
	){
	return Lifter::Status::ERROR;
    } 
    if(status_detail.at_top){
	return Lifter::Status::TOP;
    }
    if(status_detail.at_bottom){
	return Lifter::Status::BOTTOM;
    }
    if(status_detail.at_climbed_height){
	return Lifter::Status::CLIMBED;
    }
    return Lifter::Status::MIDDLE;
}

bool ready(Lifter::Status const& status,Lifter::Goal const& goal){
    switch(goal.mode()){
    case Lifter::Goal::Mode::CLIMB:
	return status == Lifter::Status::CLIMBED;
    case Lifter::Goal::Mode::UP:
	return status == Lifter::Status::TOP;
    case Lifter::Goal::Mode::DOWN:
	return status == Lifter::Status::BOTTOM;
    case Lifter::Goal::Mode::STOP:
	return true;
    case Lifter::Goal::Mode::GO_TO_HEIGHT:
    case Lifter::Goal::Mode::GO_TO_PRESET:
	return Lifter::lifter_controller.done();
    default:
	nyi
	    }
}

#ifdef LIFTER_TEST
#include "formal.h"

int main(){
    Lifter lifter;
    tester(lifter);
}

#endif
