#include "teleop.h"
#include <math.h>
#include "autonomous.h"
#include "../input/util.h"

using namespace std;

double set_drive_speed(double axis,double boost,bool slow){
	static const float MAX_SPEED=1;//Change this value to change the max power the robot will achieve with full boost (cannot be larger than 1.0)
	static const float DEFAULT_SPEED=.4;//Change this value to change the default power
	static const float SLOW_BY=.5;//Change this value to change the percentage of the default power the slow button slows
	return (pow(axis,3)*((DEFAULT_SPEED+(MAX_SPEED-DEFAULT_SPEED)*boost)-((DEFAULT_SPEED*SLOW_BY)*slow)));
}

bool operator<(Teleop::Nudge const& a,Teleop::Nudge const& b){
	#define X(A,B) if(a.B<b.B) return 1; if(b.B<a.B) return 0;
	NUDGE_ITEMS(X)
	#undef X
	return 0;
}

bool operator==(Teleop::Nudge const& a,Teleop::Nudge const& b){
	#define X(A,B) if(a.B!=b.B) return 0;
	NUDGE_ITEMS(X)
	#undef X
	return 1;
}

ostream& operator<<(ostream& o,Teleop::Nudge const& a){
	#define X(t,NAME) o<<""#NAME<<":"<<(a.NAME)<<" ";
	NUDGE_ITEMS(X)
	#undef X
	return o;
}

ostream& operator<<(ostream& o,Teleop::Collector_mode const& a){
	#define X(NAME) if(a==Teleop::Collector_mode::NAME) return o<<""#NAME;
	COLLECTOR_MODES
	#undef X
	assert(0);
}

Executive Teleop::next_mode(Next_mode_info info) {
	if (info.autonomous_start) {
		return Executive{Autonomous()};
	}
	//Teleop t(CONSTRUCT_STRUCT_PARAMS(TELEOP_ITEMS));
	return Executive{*this};
}

IMPL_STRUCT(Teleop::Teleop,TELEOP_ITEMS)

Teleop::Teleop():lifter_goal(Lifter::Goal::stop()),wings_goal(Wings::Goal::LOCKED),collector_mode(Collector_mode::DO_NOTHING),started_intake_with_cube(false),high_gear(false){}

Toplevel::Goal Teleop::run(Run_info info) {
	messageLogger &logger = messageLogger::get();
	logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_TELEOP);
	logger << "Teleop:\n";

	Toplevel::Goal goals;
	
	bool enabled = info.in.robot_mode.enabled;

	{//Set drive goals
		bool spin=fabs(info.driver_joystick.axis[Gamepad_axis::RIGHTX])>.01;//drive turning button
		double boost=info.driver_joystick.axis[Gamepad_axis::LTRIGGER]; //Turbo button
		bool slow=info.driver_joystick.button[Gamepad_button::LB]; //Slow button

		POV_section driver_pov=pov_section(info.driver_joystick.pov);
		
		const array<POV_section,NUDGES> nudge_povs={POV_section::UP,POV_section::DOWN,POV_section::RIGHT,POV_section::LEFT};
		//Forward, backward, clockwise, counter-clockwise
		for(unsigned i=0;i<NUDGES;i++){
			if(nudges[i].trigger(boost<.25 && driver_pov==nudge_povs[i])) nudges[i].timer.set(.1);
			nudges[i].timer.update(info.in.now,enabled);
		}
		const double NUDGE_POWER=.2,ROTATE_NUDGE_POWER=.2;
		double left=([&]{
			if(!nudges[Nudges::FORWARD].timer.done()) return NUDGE_POWER;
			if(!nudges[Nudges::BACKWARD].timer.done()) return -NUDGE_POWER;
			if(!nudges[Nudges::CLOCKWISE].timer.done()) return ROTATE_NUDGE_POWER;
			if(!nudges[Nudges::COUNTERCLOCKWISE].timer.done()) return -ROTATE_NUDGE_POWER;
			double power=set_drive_speed(info.driver_joystick.axis[Gamepad_axis::LEFTY],boost,slow);
			if(spin) power+=set_drive_speed(-info.driver_joystick.axis[Gamepad_axis::RIGHTX],boost,slow);
			return -power; //inverted so drivebase values can be positive
		}());
		double right=-clip([&]{ //right side is reversed
			if(!nudges[Nudges::FORWARD].timer.done()) return -NUDGE_POWER;
			if(!nudges[Nudges::BACKWARD].timer.done()) return NUDGE_POWER;
			if(!nudges[Nudges::CLOCKWISE].timer.done()) return ROTATE_NUDGE_POWER;	
			if(!nudges[Nudges::COUNTERCLOCKWISE].timer.done()) return -ROTATE_NUDGE_POWER;
			double power=set_drive_speed(info.driver_joystick.axis[Gamepad_axis::LEFTY],boost,slow);
			if(spin) power-=set_drive_speed(-info.driver_joystick.axis[Gamepad_axis::RIGHTX],boost,slow);
			return power;
		}());

		if(info.driver_joystick.button[Gamepad_button::RB]) high_gear = false;
		if(info.driver_joystick.axis[Gamepad_axis::RTRIGGER] > .8) high_gear = true;

		Drivebase::Goal::Gear gear_shifter = high_gear ? Drivebase::Goal::Gear::HIGH : Drivebase::Goal::Gear::LOW;

		goals.drive = Drivebase::Goal::absolute(left, right, gear_shifter, false);
	}

	switch(collector_mode) {
		case Collector_mode::DO_NOTHING:
			goals.grabber = Grabber::Goal::stop();
			goals.intake = Intake::Goal::off();
			break;
		case Collector_mode::GRABBING:
			goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::CLOSED);
			goals.intake = Intake::Goal::off();
			break;
		case Collector_mode::COLLECT_OPEN:
			goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::OPEN);
			goals.intake = Intake::Goal::in();
			if(info.status.grabber.has_cube) collector_mode = Collector_mode::GRABBING;
			break;
		case Collector_mode::COLLECT_CLOSED:
			goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::CLOSED);
			goals.intake = Intake::Goal::in();
			if(info.status.grabber.has_cube) collector_mode = Collector_mode::GRABBING;
			break;
		case Collector_mode::EJECT:
		case Collector_mode::DROP:
			if(collector_mode == Collector_mode::EJECT) {
				goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::CLOSED);
				goals.intake = Intake::Goal::out();
			} else {
				goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::OPEN);
				goals.intake = Intake::Goal::out(0.2);
			}
			intake_timer.update(info.in.now, info.in.robot_mode.enabled);
			if((started_intake_with_cube && !info.status.grabber.has_cube) || intake_timer.done())
				collector_mode = Collector_mode::DO_NOTHING;
			break;
		default: assert(0);
	}
	logger << "Collector: " << collector_mode << "\n";
	logger << "Intake: " << goals.intake << "\n";
	logger << "Grabber: " << goals.grabber << "\n";

	if(has_cube_trigger(info.status.grabber.has_cube)) {
		collector_mode = Collector_mode::DO_NOTHING;
		lifter_goal = Lifter::Goal::go_to_preset(LifterController::Preset::EXCHANGE);
	}

	if(info.panel.floor) {
		lifter_goal = Lifter::Goal::go_to_preset(LifterController::Preset::FLOOR);
		collector_mode = Collector_mode::DO_NOTHING;
	} else if(info.panel.exchange) {
		lifter_goal = Lifter::Goal::go_to_preset(LifterController::Preset::EXCHANGE);
		collector_mode = Collector_mode::DO_NOTHING;
	} else if(info.panel.switch_) {
		lifter_goal = Lifter::Goal::go_to_preset(LifterController::Preset::SWITCH);
		collector_mode = Collector_mode::DO_NOTHING;
	} else if(info.panel.scale) {
		lifter_goal = Lifter::Goal::go_to_preset(LifterController::Preset::SCALE);
		collector_mode = Collector_mode::DO_NOTHING;
	} else if(info.panel.lifter == Panel::Lifter::UP) {
		lifter_goal = Lifter::Goal::up(info.panel.lifter_high_power);
		collector_mode = Collector_mode::DO_NOTHING;
	} else if(info.panel.lifter == Panel::Lifter::DOWN) {
		lifter_goal = Lifter::Goal::down(info.panel.lifter_high_power);
		collector_mode = Collector_mode::DO_NOTHING;
	} else if(ready(status(info.status.lifter), lifter_goal)) {
		lifter_goal = Lifter::Goal::stop();
	}
	goals.lifter = lifter_goal;

	Lifter::Goal prep_climb_goal = Lifter::Goal::go_to_preset(LifterController::Preset::PREP_CLIMB);
	Lifter::Goal climb_goal = Lifter::Goal::climb();
	bool prep_climb_done = ready(status(info.status.lifter), prep_climb_goal);
	bool climb_done = ready(status(info.status.lifter), climb_goal);
	if(info.panel.climb) {
		if(!prep_climb_done) {
			logger << "PREPARING TO CLIMB\n";

			lifter_goal = prep_climb_goal;
		} else if(info.panel.climb_lock) {
			logger << "CLIMBING\n";

			goals.lifter = Lifter::Goal::climb();
		}
	}
	if(goals.lifter == prep_climb_goal || prep_climb_done)
		goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::STOWED);
	if(prep_climb_done)
		goals.lifter = Lifter::Goal::low_gear();
	if(climb_done)
		goals.lifter = Lifter::Goal::lock(true);

	if(info.panel.collect_open) collector_mode = Collector_mode::COLLECT_OPEN;
	if(info.panel.collect_closed) collector_mode = Collector_mode::COLLECT_CLOSED;
	if(info.panel.eject) {
		collector_mode = Collector_mode::EJECT;
		started_intake_with_cube = info.status.grabber.has_cube;
		intake_timer.set(1.0);
	}
	if(info.panel.drop) {
		collector_mode = Collector_mode::DROP;
		started_intake_with_cube = info.status.grabber.has_cube;
		intake_timer.set(0.5);
	}

	if(calibrate_trigger(info.panel.calibrate)) {
		Lifter::lifter_controller.setCalibrate(true);
		Grabber::grabber_controller.setDoneCalibrating(false);
	} else {
		Lifter::lifter_controller.setCalibrate(false);	
	}
	if(info.panel.calibrate) {
		goals.lifter = Lifter::Goal::calibrate();
		goals.grabber = Grabber::Goal::calibrate();
	}

	//if(info.panel.wings && info.panel.climb_lock) wings_goal = Wings::Goal::UNLOCKED;
	//goals.wings = wings_goal;

	if(info.panel.wings) goals.lifter = Lifter::Goal::lock();

	if(!info.panel.grabber_auto) {
		if(info.panel.grabber == Panel::Grabber::OFF) goals.grabber = Grabber::Goal::stop();
		if(info.panel.grabber == Panel::Grabber::OPEN) goals.grabber = Grabber::Goal::open();
		if(info.panel.grabber == Panel::Grabber::CLOSE) goals.grabber = Grabber::Goal::close();
	}

	if(!info.panel.intake_auto) {
		if(info.panel.intake == Panel::Intake::OFF) goals.intake = Intake::Goal::off();
		if(info.panel.intake == Panel::Intake::IN) goals.intake = Intake::Goal::in();
		if(info.panel.intake == Panel::Intake::OUT) goals.intake = Intake::Goal::out();
	}

	{
		goals.lights.climbing = false;//TODO
		goals.lights.lifter_height = (int)(info.status.lifter.height / 3);
	}

	logger.endMessage();
	
	return goals;
}

bool Teleop::operator<(Teleop const& a)const{
	#define X(t,NAME) if(NAME<a.NAME) return 1; if(a.NAME<NAME) return 0;
	TELEOP_ITEMS(X)
	#undef X
	return 0;
}

bool Teleop::operator==(Teleop const& a)const{
	#define X(t,NAME) if(NAME!=a.NAME) return 0;
	TELEOP_ITEMS(X)
	#undef X
	return 1;
}

void Teleop::display(ostream& o)const{
	o<<"Teleop( ";
	#define X(t,NAME) o<<""#NAME<<":"<<(NAME)<<" ";
	TELEOP_ITEMS(X)
	#undef X
	o<<")";
}

#ifdef TELEOP_TEST
#include "test.h"

int main() {
	Teleop a;
	test_executive(a);
}
#endif
