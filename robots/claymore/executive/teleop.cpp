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

Teleop::Teleop():lifter_goal(Lifter::Goal::stop()),wings_goal(Wings::Goal::LOCKED),collector_mode(Collector_mode::DO_NOTHING),print_number(0){}

Toplevel::Goal Teleop::run(Run_info info) {
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

		//
		// const Talon_srx_output::Speed_mode DRIVE_SPEED_MODE = info.driver_joystick.button[Gamepad_button::A] ?  Talon_srx_output::Speed_mode::COAST : Talon_srx_output::Speed_mode::BRAKE;
		//
		// TODO - how are we using the DRIVE_SPEED_MODE ???
		//
		goals.drive = Drivebase::Goal::absolute(left,right);
	}

	goals.gear_shifter=[&]{
		if(info.driver_joystick.button[Gamepad_button::RB]) return Gear_shifter::Goal::LOW;
		if(info.driver_joystick.axis[Gamepad_axis::RTRIGGER]>.8) return Gear_shifter::Goal::HIGH;
		return Gear_shifter::Goal::AUTO;
	}();	

	switch(collector_mode) {
		case Collector_mode::DO_NOTHING:
			goals.grabber = Grabber::Goal::stop();
			goals.intake = Intake::Goal::OFF;
			break;
		case Collector_mode::GRABBING:
			goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::CLOSED);
			goals.intake = Intake::Goal::OFF;
			break;
		case Collector_mode::COLLECT_OPEN:
			goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::OPEN);
			goals.intake = Intake::Goal::IN;
			if(info.status.grabber.has_cube) collector_mode = Collector_mode::GRABBING;
			break;
		case Collector_mode::COLLECT_CLOSED:
			goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::CLOSED);
			goals.intake = Intake::Goal::IN;
			if(info.status.grabber.has_cube) collector_mode = Collector_mode::GRABBING;
			break;
		case Collector_mode::EJECT:
			goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::CLOSED);
			goals.intake = Intake::Goal::OUT;
			eject_timer.update(info.in.now, info.in.robot_mode.enabled);
			if(eject_timer.done()) collector_mode = Collector_mode::DO_NOTHING;
			break;
		case Collector_mode::DROP:
			goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::OPEN);
			goals.intake = Intake::Goal::OFF;
			break;
		default: assert(0);
	}
	std::cout << "Collector: " << collector_mode << endl;
	//std::cout << "Intake: " << goals.intake << endl;
	//std::cout << "Grabber: " << goals.grabber << endl;

	if(info.panel.lifter == Panel::Lifter::OFF && ready(status(info.status.lifter), lifter_goal)) {
		lifter_goal = Lifter::Goal::stop();
		//collector_mode = Collector_mode::DO_NOTHING;
	}
	if(info.panel.floor) {
		lifter_goal = Lifter::Goal::go_to_preset(LifterController::Preset::FLOOR);
		collector_mode = Collector_mode::DO_NOTHING;
	}
	if(info.panel.exchange) {
		lifter_goal = Lifter::Goal::go_to_preset(LifterController::Preset::EXCHANGE);
		collector_mode = Collector_mode::DO_NOTHING;
	}
	if(info.panel.switch_) {
		lifter_goal = Lifter::Goal::go_to_preset(LifterController::Preset::SWITCH);
		collector_mode = Collector_mode::DO_NOTHING;
	}
	if(info.panel.scale) {
		lifter_goal = Lifter::Goal::go_to_preset(LifterController::Preset::SCALE);
		collector_mode = Collector_mode::DO_NOTHING;
	}
	if(info.panel.lifter == Panel::Lifter::UP) {
		lifter_goal = Lifter::Goal::up(info.panel.lifter_high_power);
		collector_mode = Collector_mode::DO_NOTHING;
	}
	if(info.panel.lifter == Panel::Lifter::DOWN) {
		lifter_goal = Lifter::Goal::down(info.panel.lifter_high_power);
		collector_mode = Collector_mode::DO_NOTHING;
	}
	goals.lifter = lifter_goal;

	if(info.panel.climb) {
		Lifter::Goal prep_climb_goal = Lifter::Goal::go_to_preset(LifterController::Preset::PREP_CLIMB);
		if(!ready(status(info.status.lifter), prep_climb_goal)) {
			std::cout << "NOT READY" << endl;
			lifter_goal = prep_climb_goal;
		}
		else if(info.panel.climb_lock) {
			std::cout << "CLIMBING" << endl;
			goals.lifter = Lifter::Goal::climb();
		}
	}

	if(info.panel.collect_open) collector_mode = Collector_mode::COLLECT_OPEN;
	if(info.panel.collect_closed) collector_mode = Collector_mode::COLLECT_CLOSED;
	if(info.panel.eject) {
		collector_mode = Collector_mode::EJECT;
		eject_timer.set(2);
	}
	if(info.panel.drop) collector_mode = Collector_mode::DROP;

	//std::cout << "goal: " << goals.lifter << endl;

	if(info.panel.wings && info.panel.climb_lock) wings_goal = Wings::Goal::UNLOCKED;
	goals.wings = wings_goal;

	if(!info.panel.grabber_auto) {
		if(info.panel.grabber == Panel::Grabber::OFF) goals.grabber = Grabber::Goal::stop();
		if(info.panel.grabber == Panel::Grabber::OPEN) goals.grabber = Grabber::Goal::open();//go_to_preset(GrabberController::Preset::OPEN);
		if(info.panel.grabber == Panel::Grabber::CLOSE) goals.grabber = Grabber::Goal::close();//go_to_preset(GrabberController::Preset::CLOSED);
	}

	//std::cout << "Grabber: " << goals.grabber << endl;

	if(!info.panel.intake_auto) {
		if(info.panel.intake == Panel::Intake::OFF) goals.intake = Intake::Goal::OFF;
		if(info.panel.intake == Panel::Intake::IN) goals.intake = Intake::Goal::IN;
		if(info.panel.intake == Panel::Intake::OUT) goals.intake = Intake::Goal::OUT;
	}

	{
		goals.lights.climbing = false;//TODO
		goals.lights.lifter_height = (int)(info.status.lifter.height / 3);
	}
	
	#ifdef PRINT_OUTS
	if(info.in.ds_info.connected && (print_number%10)==0){
		cout<<"\nencoders:"<<info.status.drive<<"\n";
		if(info.in.camera.enabled){
			cout<<"size: "<<info.in.camera.blocks.size()<<" blocks: "<<info.in.camera.blocks<<"\n";
			/*for (vector<Pixy::Block>::const_iterator it=info.in.camera.blocks.begin();it!=info.in.camera.blocks.end();it++){
				cout<<"\tarea: "<<(it->width * it->height)<<"\n";
			}*/
		}
		cout<<"\n";
	}
	print_number++;
	#endif
	
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
