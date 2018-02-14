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

	#define COLLECTOR_MODES X(DO_NOTHING) X(GRABBING) X(COLLECT_OPEN) X(COLLECT_CLOSED) X(EJECT) X(DROP)
	enum class Collector_mode{
		#define X(NAME) NAME,
		COLLECTOR_MODES
		#undef X
	};

	#define TELEOP_ITEMS(X)\
		X(SINGLE_ARG(std::array<Nudge,NUDGES>),nudges) \
		X(Lifter::Goal,lifter_goal) \
		X(Wings::Goal,wings_goal) \
		X(Collector_mode, collector_mode) \
		X(Countdown_timer, eject_timer) \
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
//std::ostream& operator<<(std::ostream&,Teleop::Intake_mode const&);

bool operator<(Teleop::Nudge const&,Teleop::Nudge const&);
bool operator==(Teleop::Nudge const&,Teleop::Nudge const&);
double set_drive_speed(double,double,double);

#endif
