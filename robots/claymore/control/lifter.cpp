#include "lifter.h"
#include "util.h"
#include "subsystems.h"
#include "message_logger.h"
#include <cmath>

using namespace std;

#define LIFTER_ADDRESS_L 0
#define LIFTER_ADDRESS_R 1
#define LIFTER_SHIFTER 1

#define BOTTOM_HALL_EFFECT_ADDRESS 9
#define TOP_HALL_EFFECT_ADDRESS 8
#define ENCODER_ADDRESS 2
#define ENCODER_DIOS 4, 5

LifterController Lifter::lifter_controller;

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

Lifter::Goal Lifter::Goal::background(){
    Lifter::Goal a;
    a.mode_ = Lifter::Goal::Mode::BACKGROUND;
    a.gearing_ = Lifter::Goal::Gearing::HIGH;
    return a;
}

Lifter::Goal Lifter::Goal::calibrate(){
    Lifter::Goal a;
    a.mode_ = Lifter::Goal::Mode::CALIBRATE;
    a.gearing_ = Lifter::Goal::Gearing::HIGH;
    return a;
}

Lifter::Goal Lifter::Goal::low_gear(){
    Lifter::Goal a;
    a.mode_ = Lifter::Goal::Mode::LOW_GEAR;
    a.gearing_ = Lifter::Goal::Gearing::LOW;
    return a;
}

Lifter::Output::Output(double p, Lifter::Output::Gearing g):power(p),gearing(g){}
Lifter::Output::Output():Output(0,Lifter::Output::Gearing::HIGH){}

Lifter::Input::Input(bool b, bool t, int e):bottom_hall_effect(b),top_hall_effect(t),ticks(e){}
Lifter::Input::Input():Input(false,false,0){}

Lifter::Status_detail::Status_detail(bool b,bool t,bool c,bool usr,bool lsr,double h,double ti,double dt):at_bottom(b),at_top(t),at_climbed_height(c),upper_slowdown_range(usr),lower_slowdown_range(lsr),height(h),time(ti),dt(dt){}
Lifter::Status_detail::Status_detail():Status_detail(false,false,false,false,false,0.0,0.0,0.0){}

Lifter::Estimator::Estimator(Lifter::Status_detail s,Output::Gearing g, double cg, double eo):last(s),last_gearing(g),climb_goal(cg),encoder_offset(eo){}
Lifter::Estimator::Estimator():Estimator(Lifter::Status_detail{},Output::Gearing::HIGH,0.0,0.0){}

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
    r.pwm[LIFTER_ADDRESS_L] = -out.power;
    r.pwm[LIFTER_ADDRESS_R] = -out.power;
    r.solenoid[LIFTER_SHIFTER] = out.gearing == Lifter::Output::Gearing::LOW;

    auto set_encoder=[&](unsigned int a, unsigned int b,unsigned int loc){
	r.digital_io[a] = Digital_out::encoder(loc,1);
	r.digital_io[b] = Digital_out::encoder(loc,0);
    };
    set_encoder(ENCODER_DIOS, ENCODER_ADDRESS);

    return r;
};

Lifter::Output Lifter::Output_applicator::operator()(Robot_outputs const& r)const{
    return {
	-r.pwm[LIFTER_ADDRESS_L], //assuming that left and right sides are set to the same value
	r.solenoid[LIFTER_SHIFTER] ? Lifter::Output::Gearing::LOW : Lifter::Output::Gearing::HIGH
	   };
}

Robot_inputs Lifter::Input_reader::operator()(Robot_inputs r, Lifter::Input const& in)const{
    r.digital_io.in[BOTTOM_HALL_EFFECT_ADDRESS] = in.bottom_hall_effect ? Digital_in::_0 : Digital_in::_1; //Active low
    r.digital_io.in[TOP_HALL_EFFECT_ADDRESS] = in.top_hall_effect ? Digital_in::_0 : Digital_in::_1;
    r.digital_io.encoder[ENCODER_ADDRESS] = -in.ticks;
    return r;
}

Lifter::Input Lifter::Input_reader::operator()(Robot_inputs const& r)const{
    int enc_val = 9999;
    if(r.digital_io.encoder[ENCODER_ADDRESS])
	enc_val = *(r.digital_io.encoder[ENCODER_ADDRESS]);
    enc_val = -enc_val;
    Lifter::Input result = Lifter::Input(
	r.digital_io.in[BOTTOM_HALL_EFFECT_ADDRESS] == Digital_in::_0,
	r.digital_io.in[TOP_HALL_EFFECT_ADDRESS] == Digital_in::_0,
	enc_val
    );
    return result;
}

void Lifter::Estimator::update(Time const& now, Lifter::Input const& in, Lifter::Output const& out){
    messageLogger &logger = messageLogger::get();
    logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER);

    logger << "Lifter:\n";

    paramsInput* input_params = Lifter::lifter_controller.getParams();

    if(Lifter::lifter_controller.calibrating()) encoder_offset = in.ticks;

    /*
    const double TICKS_PER_MOTOR_REVOLUTION = 12.0;
    const double MOTOR_REVS_PER_LIFTER_REV = 100.0; //Gear ratio TODO: NEEDS CORRECT VALUE
    const double TICKS_PER_LIFTER_REVOLUTION = TICKS_PER_MOTOR_REVOLUTION * MOTOR_REVS_PER_LIFTER_REV;
    const double LIFTER_GEAR_DIAMETER = 6.0; //inches
    const double LIFTER_GEAR_CIRCUMFERENCE = LIFTER_GEAR_DIAMETER * PI;
    const double INCHES_PER_TICK = LIFTER_GEAR_CIRCUMFERENCE / TICKS_PER_LIFTER_REVOLUTION;
    */
    const double INCHES_PER_TICK_HIGH_GEAR = .08327; //(1.0 / 11.4201); //From Jeff
    //const double INCHES_PER_TICK_LOW_GEAR = 4442.41 * 12.0; //From Jeff
    const double COLLECTOR_OFFSET = 11.375; //inches
    last.height = (in.ticks - encoder_offset) * INCHES_PER_TICK_HIGH_GEAR + COLLECTOR_OFFSET;

    double top_limit = input_params->getValue("lifter:height:top_limit", 96.0);
    double bottom_limit = input_params->getValue("lifter:collector_offset", 11.375) + 0.75;

    last.at_bottom = in.bottom_hall_effect || last.height < bottom_limit;
    last.at_top = in.top_hall_effect || last.height > top_limit;

    double slowdown_range = input_params->getValue("lifter:slowdown_range", 6.0);
    last.upper_slowdown_range = last.height > (top_limit - slowdown_range);
    last.lower_slowdown_range = last.height < (bottom_limit + slowdown_range);

    if(out.gearing == Output::Gearing::LOW && out.gearing != last_gearing) climb_goal = last.height - input_params->getValue("lifter:climbing_difference", 100.0);
    last.at_climbed_height = last.height < climb_goal;
    last_gearing = out.gearing;
	
    last.dt = now - last.time;
    last.time = now;

    logger << "Limit Switches: Top: " << in.top_hall_effect << "   Bottom: " << in.bottom_hall_effect << "\n";
    logger << "Upper slowdown range: " << last.upper_slowdown_range << "   Lower slowdown range: " << last.lower_slowdown_range << "\n";
    logger << "Ticks: " << in.ticks << "    Height: " << last.height;

    logger.endMessage();
}

Lifter::Status_detail Lifter::Estimator::get()const{
    return last;
}

set<Lifter::Input> examples(Lifter::Input*){
    return {
	{false,false,0},
	{true,false,0},
	{false,true,0},
	{true,true,0},
    };
}

set<Lifter::Output> examples(Lifter::Output*){
    return {
	{0.0, Lifter::Output::Gearing::HIGH}, 
	{0.0, Lifter::Output::Gearing::LOW}
    };
}

set<Lifter::Status_detail> examples(Lifter::Status_detail*){
    return {
	{false,false,false,false,false,0.0,0.0,0.0},
	{false,false,true,false,false,0.0,0.0,0.0},
	{true,false,false,false,false,0.0,0.0,0.0},
	{true,false,true,false,false,0.0,0.0,0.0},
	{false,true,false,false,false,0.0,0.0,0.0},
	{false,true,true,false,false,0.0,0.0,0.0},
	{true,true,false,false,false,0.0,0.0,0.0},
	{true,true,true,false,false,0.0,0.0,0.0}
    };
}

set<Lifter::Status> examples(Lifter::Status*){
    return {Lifter::Status::CLIMBED,Lifter::Status::MIDDLE,Lifter::Status::BOTTOM,Lifter::Status::TOP,Lifter::Status::ERROR};
}

set<Lifter::Goal> examples(Lifter::Goal*){
    return {Lifter::Goal::climb(),Lifter::Goal::up(),Lifter::Goal::stop(),Lifter::Goal::down(),Lifter::Goal::go_to_height(0.0)};
}

Lifter::Output control(Lifter::Status_detail const& status_detail, Lifter::Goal const& goal){
    paramsInput* input_params = Lifter::lifter_controller.getParams();

    Lifter::Status s = status(status_detail);

    Lifter::Output out = {0.0, goal.gearing()};
    if(s == Lifter::Status::ERROR) return out;

    if(Lifter::lifter_controller.runningInBackground() || goal.mode() == Lifter::Goal::Mode::BACKGROUND) {
	Lifter::lifter_controller.update(status_detail.height, status_detail.time, status_detail.dt, out.power);
	return out;
    }

    switch(goal.mode()){
    case Lifter::Goal::Mode::CLIMB:
	out.power = -input_params->getValue("lifter:climb_power", 0.6);
	break;
    case Lifter::Goal::Mode::UP:
	out.power = goal.high_power() ? input_params->getValue("lifter:manual_power:high", 0.8) : input_params->getValue("lifter:manual_power:low", 0.4);
	break;
    case Lifter::Goal::Mode::DOWN:
	out.power = goal.high_power() ? -input_params->getValue("lifter:manual_power:high", 0.8) : -input_params->getValue("lifter:manual_power:low", 0.4);
	break;
    case Lifter::Goal::Mode::STOP:
	Lifter::lifter_controller.idle(status_detail.height, status_detail.time, status_detail.dt);
	out.power = input_params->getValue("lifter:hold_power:high_gear", 0.1);
	break; 
    case Lifter::Goal::Mode::GO_TO_HEIGHT:
	Lifter::lifter_controller.updateHeightOnChange(goal.target(), status_detail.height, status_detail.time);
	Lifter::lifter_controller.update(status_detail.height, status_detail.time, status_detail.dt, out.power);
	break;
    case Lifter::Goal::Mode::GO_TO_PRESET:
	Lifter::lifter_controller.updateHeightOnChange(goal.preset_target(), status_detail.height, status_detail.time);
	Lifter::lifter_controller.update(status_detail.height, status_detail.time, status_detail.dt, out.power);
	break;
    case Lifter::Goal::Mode::BACKGROUND:
	break;
    case Lifter::Goal::Mode::CALIBRATE:
	break;
    case Lifter::Goal::Mode::LOW_GEAR:
	out.power = input_params->getValue("lifter:hold_power:low_gear", 0.0);
	break;
    default:
	nyi
    }

    messageLogger &logger = messageLogger::get();
    logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER);
    logger << "Lifter status: " << status_detail.at_top << " " << status_detail.at_bottom << " " << out.power << "\n";

    if((status_detail.upper_slowdown_range && out.power > 0.0) ||
       (status_detail.lower_slowdown_range && out.power < 0.0))
	out.power = copysign(min(fabs(out.power), input_params->getValue("lifter:slowdown_power", 0.2)), out.power);

    if((status_detail.at_top && out.power > 0.0) ||
       (status_detail.at_bottom && out.power < 0.0) ||
       (status_detail.at_climbed_height && out.power < 0.0))
	out.power = 0.0;

    logger << "After: " << out.power << "\n";
    logger.endMessage();

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
    case Lifter::Goal::Mode::DOWN:
    case Lifter::Goal::Mode::STOP:
    case Lifter::Goal::Mode::CALIBRATE:
    case Lifter::Goal::Mode::LOW_GEAR:
	return true;
    case Lifter::Goal::Mode::GO_TO_HEIGHT:
    case Lifter::Goal::Mode::GO_TO_PRESET:
    case Lifter::Goal::Mode::BACKGROUND:
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
