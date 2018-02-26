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

ostream& operator<<(ostream& o, Teleop::HasCubeState const &a)
{
    switch(a)
    {
    case Teleop::HasCubeState::NoCube:
		o << "NoCube" ;
		break ;
    case Teleop::HasCubeState::MaybeHasCube:
		o << "MaybeHasCube" ;
		break ;
    case Teleop::HasCubeState::HasCube:
		o << "HasCube" ;
		break ;
    case Teleop::HasCubeState::MaybeLostCube:
		o << "MaybeLostCube" ;
		break ;
    }

    return o ;
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

Teleop::Teleop():lifter_goal(Lifter::Goal::stop()),wings_goal(Wings::Goal::LOCKED),collector_mode(Collector_mode::DO_NOTHING),started_intake_with_cube(false),high_gear(false)
{
    has_cube_state = HasCubeState::NoCube ;
}

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
	//logger << "Collector: " << collector_mode << "\n";
	//logger << "Intake: " << goals.intake << "\n";
	//logger << "Grabber: " << goals.grabber << "\n";

	//
	// Note, this is a state machine
	//
	// States:
	//
	// NoCube        - the collector is not holding a cube
	// MaybeHasCube  - the collector may be holding a cube but we want the has_cube
	//                 signal to stay active long enough to be sure (250 ms)
	// HasCube       - the collector is holding a cube
	// MaybeLostCube - the collector was previously holding a cube and may have lost it
	//                 we want the has_cube signal to stay inactive long enough to be
	//                 sure
	//

	switch(has_cube_state)
	{
	case HasCubeState::NoCube:
	    if (info.status.grabber.has_cube)
	    {
			//
			// We are currently in the no cube state, but see the cube
			// present signal, we move immediately to the HasCube state.
			//
			has_cube_state = HasCubeState::MaybeHasCube ;
			cube_timer.set(0.25) ;
	    }
	    break ;

	case HasCubeState::MaybeHasCube:
	    if (!info.status.grabber.has_cube)
	    {
			//
			// The has cube disappeared before the timer expired.  We assume
			// this was a glitch and do not respond to the cube
			//
			has_cube_state = HasCubeState::NoCube ;
	    }
	    else if (cube_timer.done())
	    {
			//
			// The timer for the has cube status has expired, assume we really
			// have a cube.
			//

			//
			// Note, we shift the collector to holding a cube regardless of where
			// the lifter is on the robot.  This keeps the grabber holding the cube
			// and shuts down the intake belts
			//
			collector_mode = Collector_mode::DO_NOTHING;

			if (Lifter::lifter_controller.nearPreset(LifterController::Preset::FLOOR, info.status.lifter.height, 2.0) &&
				Lifter::lifter_controller.isCalibrated())
			{
				//
				// If we collected the cube witin a small tolerance of the floor height, we move the
				// lifter up to EXCHANGE height.  Note, if we collect the cube at any other height
				// it is already off the floor and we let the drive team deal with height.
				//
				// Also, if the lifter has not been calibrated, we never move the lifter.  We want it
				// to stay on the floor until the lifter is calibrated.
				//
				lifter_goal = Lifter::Goal::go_to_preset(LifterController::Preset::EXCHANGE);
			}
			has_cube_state = HasCubeState::HasCube ;
	    }
	    break ;
	case HasCubeState::HasCube:
	    if (!info.status.grabber.has_cube)
	    {
			//
			// We lost the has cube signal, so we may have lost the cube, or the
			// signal may have just disappeared temporarily.  Start a timer to see if the
			// cube is really gone
			//
			cube_timer.set(0.5) ;
			has_cube_state = HasCubeState::MaybeLostCube ;
	    }
	    break ;
	case HasCubeState::MaybeLostCube:
	    if (info.status.grabber.has_cube)
	    {
			//
			// The cube has reappeared, just assume some flakiness with the sensor of that
			// the cube is jostling around.
			//
			has_cube_state = HasCubeState::HasCube ;
	    }
	    else if (cube_timer.done())
	    {
			//
			// The cube is really gone, go back to the no cube state
			//
			has_cube_state = HasCubeState::NoCube ;
	    }
	    break ;
	} ;
	cube_timer.update(info.in.now, info.in.robot_mode.enabled);

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
	if(info.panel.climb) {
		if(!prep_climb_done) {
			logger << "PREPARING TO CLIMB\n";

			lifter_goal = prep_climb_goal;
		} else if(info.panel.climb_lock) {
			logger << "CLIMBING\n";

			goals.lifter = climb_goal;
		}
	}
	if(goals.lifter == prep_climb_goal || prep_climb_done) {
		logger << "A\n";
		goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::STOWED);
	}
	if(goals.lifter == prep_climb_goal && prep_climb_done) {
		logger << "B\n";
		goals.lifter = Lifter::Goal::low_gear();
	}
	logger << "At Climb Height: " << info.status.lifter.at_climbed_height << "\n";
	if(info.status.lifter.at_climbed_height){
		logger << "C\n";
		goals.lifter = Lifter::Goal::lock(true);
	}

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
		goals.lights.climbing = goals.lifter.preset_target() == LifterController::Preset::PREP_CLIMB;
		goals.lights.lifter_height = (unsigned)info.status.lifter.height;
		goals.lights.drive_left = goals.drive.left();
		goals.lights.drive_right = goals.drive.right();
		goals.lights.has_cube = info.status.grabber.has_cube;
		goals.lights.collector_open = collector_mode == Collector_mode::COLLECT_OPEN;
		goals.lights.collector_closed = collector_mode == Collector_mode::COLLECT_CLOSED;
		goals.lights.wings_deployed = goals.wings == Wings::Goal::UNLOCKED;
		goals.lights.lifter_status = [&]{
			Lifter::Status s = status(info.status.lifter);
			if(!ready(s,goals.lifter)){
				return Lights::Goal::Lifter_status::UNKNOWN;
			}
			switch(goals.lifter.preset_target()){
			case LifterController::Preset::FLOOR:
			return Lights::Goal::Lifter_status::FLOOR;
			case LifterController::Preset::EXCHANGE:
			return Lights::Goal::Lifter_status::EXCHANGE;
			case LifterController::Preset::PREP_CLIMB:
			return Lights::Goal::Lifter_status::CLIMB;
			case LifterController::Preset::SWITCH:
			return Lights::Goal::Lifter_status::SWITCH;
			case LifterController::Preset::SCALE:
			return Lights::Goal::Lifter_status::SCALE;
			default:
			nyi
			}
		}();
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
