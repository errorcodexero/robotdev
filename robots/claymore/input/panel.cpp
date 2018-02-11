#include "panel.h"
#include <iostream>
#include <stdlib.h> 
#include "util.h"
#include "../util/util.h"
#include <cmath>

using namespace std;

const unsigned AUTO_SELECTOR_AXIS = 6;//TODO rework these constants

#define BUTTONS \
	X(floor)\
	X(exchange)\
	X(switch_)\
	X(scale)\
	X(prep_climb)\
	X(collect_closed)\
	X(collect_open)\
	X(eject)\
	X(climb)

#define THREE_POS_SWITCHES \
	X(lifter)

#define TEN_POS_SWITCHES \
	X(auto_select)
#define DIALS \

#define PANEL_ITEMS \
	BUTTONS \
	THREE_POS_SWITCHES \
	TEN_POS_SWITCHES \
	DIALS

Panel::Panel():
	in_use(0),
	#define X(BUTTON) BUTTON(false),
	BUTTONS
	#undef X
	auto_select(0)
{}

ostream& operator<<(ostream& o,Panel::Lifter a){
	#define X(NAME) if(a==Panel::Lifter::NAME) return o<<""#NAME;
	X(DOWN) X(OFF) X(UP)
	#undef X
	assert(0);
}

ostream& operator<<(ostream& o,Panel p){
	o<<"Panel(";
	o<<"in_use:"<<p.in_use;
	#define X(NAME) o<<", "#NAME":"<<p.NAME;
	PANEL_ITEMS
	#undef X
	return o<<")";
}

bool operator==(Panel const& a,Panel const& b){
	return true
	#define X(NAME) && a.NAME==b.NAME
	PANEL_ITEMS
	#undef X
	;
}

bool operator!=(Panel const& a,Panel const& b){
	return !(a==b);
}

float axis_to_percent(double a){
	return .5-(a/2);
}

bool set_button(const float AXIS_VALUE, const float LOWER_VALUE, const float TESTING_VALUE, const float UPPER_VALUE){
	float lower_tolerance = (TESTING_VALUE - LOWER_VALUE)/2;
	float upper_tolerance = (UPPER_VALUE - TESTING_VALUE)/2;
	float min = TESTING_VALUE - lower_tolerance;
	float max = TESTING_VALUE + upper_tolerance; 
	return (AXIS_VALUE > min && AXIS_VALUE < max);
}

bool get_in_use(Joystick_data d){
	for(int i = 0; i < JOY_AXES; i++) {
		if(d.axis[i] != 0) return true;
	}
	for(int i = 0; i < JOY_BUTTONS; i++) {
		if(d.button[i] != 0) return true;
	}
	return false;
}

Panel interpret_oi(Joystick_data d){
	Panel p;
	//static const float ARTIFICIAL_MAX = 1.5;
	{
		p.in_use=get_in_use(d);
		if(!p.in_use) return p;
	}
	{//set the auto mode number from the dial value
		float auto_dial_value = d.axis[AUTO_SELECTOR_AXIS];
		p.auto_select = interpret_20_turn_pot(auto_dial_value);
	}
	{//two position switches
	}
	{//three position switches
	}	
	{//buttons
	}
	{//Dials
	}
	return p;
}

Panel interpret_test_oi(Joystick_data d){	
	//static const unsigned int POTENTIOMETER_AXIS=1, TEN_POS_SWITCH_AXIS = 0; //TODO: need real values
	//static const unsigned int BUTTON_0_LOC=0, BUTTON_1_LOC=1, BUTTON_2_LOC=2, BUTTON_3_LOC=3, TWO_POS_SWITCH_0_LOC = 4, TWO_POS_SWITCH_1_LOC =5; //TODO: need real values

	Panel p;
	{
		p.in_use=[&](){
			for(int i=0;i<JOY_AXES;i++) {
				if(d.axis[i]!=0)return true;
			}
			for(int i=0;i<JOY_BUTTONS;i++) {
				if(d.button[i]!=0)return true;
			}
			return false;
		}();
		if(!p.in_use) return p;
	}
	//p. = interpret_10_turn_pot(d.axis[TEN_POS_SWITCH_AXIS]); //set the switch value from the pot value
	{//two position switches
		//p. = d.button[TWO_POS_SWITCH_0_LOC];
		//p. = d.button[TWO_POS_SWITCH_1_LOC];
	}
	{//buttons
		//p. = d.button[BUTTON_0_LOC];
		//p. = d.button[BUTTON_1_LOC];
		//p. = d.button[BUTTON_2_LOC];
		//p. = d.button[BUTTON_3_LOC];
	}
	{//dials
		//p. = d.axis[POTENTIOMETER_AXIS];
	}
	return p;
}

Panel interpret_gamepad(Joystick_data d){
	Panel p;
	p.in_use = get_in_use(d);
	if(!p.in_use) return p;
	
	//TODO: Add in all of the new controls
	p.auto_select=0;

	/*p.intake = d.button[Gamepad_button::A];
	p.eject = d.button[Gamepad_button::Y];
	p.close = d.button[Gamepad_button::X];
	p.open = d.button[Gamepad_button::B];*/

	p.lifter = Panel::Lifter::OFF;
	if(d.button[Gamepad_button::RB]) p.lifter = Panel::Lifter::UP;
	if(d.button[Gamepad_button::LB]) p.lifter = Panel::Lifter::DOWN;

	switch(pov_section(d.pov)){
		case POV_section::CENTER:
			break;
		case POV_section::UP:
			p.scale = true;
			break;
		case POV_section::UP_LEFT:
			break;
		case POV_section::LEFT:
			p.exchange = true;
			break;
		case POV_section::DOWN_LEFT:
			break;
		case POV_section::DOWN:
			p.floor = true;
			break;
		case POV_section::DOWN_RIGHT:
			break;
		case POV_section::RIGHT:
			p.switch_=true;
			break;
		case POV_section::UP_RIGHT:
			break;
		default:
			assert(0);
	}	

	return p;
}

Joystick_data driver_station_input_rand(){
	Joystick_data r;
	for(unsigned i=0;i<JOY_AXES;i++){
		r.axis[i]=(0.0+rand()%101)/100;
	}
	for(unsigned i=0;i<JOY_BUTTONS;i++){
		r.button[i]=rand()%2;
	}
	return r;
}

Panel rand(Panel*){
	return interpret_oi(driver_station_input_rand());
}

#ifdef PANEL_TEST
int main(){
	Panel p;
	for(unsigned i=0;i<50;i++){
		interpret_oi(driver_station_input_rand());
	}
	cout<<p<<"\n";
	return 0;
}
#endif
