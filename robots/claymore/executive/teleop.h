#ifndef TELEOP_H
#define TELEOP_H

#include "executive.h"
#include "../util/posedge_trigger_debounce.h"
#include "../util/posedge_toggle.h"
#include "../util/quick.h"

struct Teleop : Executive_impl<Teleop> {
	enum Nudges{FORWARD,BACKWARD,CLOCKWISE,COUNTERCLOCKWISE,NUDGES};
	#define NUDGE_ITEMS(X) X(Posedge_trigger,trigger) X(Countdown_timer,timer)
	struct Nudge{
		Posedge_trigger trigger;
		Countdown_timer timer;
	};

	#define GEAR_COLLECTOR_MODES X(STOW) X(STOW_CLOSED) X(COLLECT) X(PREP_SCORE) X(SCORE)
	enum class Gear_collector_mode{
		#define X(NAME) NAME,
		GEAR_COLLECTOR_MODES
		#undef X
	};

	#define TELEOP_ITEMS(X)\
		X(SINGLE_ARG(std::array<Nudge,NUDGES>),nudges) \
		X(int,print_number)
	STRUCT_MEMBERS(TELEOP_ITEMS)

	Executive next_mode(Next_mode_info);
	Toplevel::Goal run(Run_info);
	bool operator<(Teleop const&)const;
	bool operator==(Teleop const&)const;
	void display(std::ostream&)const;
	Teleop();
	IMPL_STRUCT_DECLARE(Teleop,TELEOP_ITEMS)
};

std::ostream& operator<<(std::ostream&,Teleop::Nudge const&);
std::ostream& operator<<(std::ostream&,Teleop::Gear_collector_mode const&);

bool operator<(Teleop::Nudge const&,Teleop::Nudge const&);
bool operator==(Teleop::Nudge const&,Teleop::Nudge const&);
double set_drive_speed(double,double,double);

#endif
