#ifndef PANEL_H
#define PANEL_H 

#include "../util/maybe.h"
#include "../util/interface.h"

struct Panel{
	static const unsigned PORT = 2;
	bool in_use;
	//Buttons:
	//2 position swicthes:
	//3 position switches:
	//10 position switches:
	int auto_select;//0-19
	//Dials:
	
	Panel();
};

bool operator!=(Panel const&,Panel const&);
std::ostream& operator<<(std::ostream&,Panel);

Panel interpret_oi(Joystick_data);
Panel interpret_test_oi(Joystick_data);
Panel interpret_gamepad(Joystick_data);

Joystick_data driver_station_input_rand();
Panel rand(Panel*);

#endif
