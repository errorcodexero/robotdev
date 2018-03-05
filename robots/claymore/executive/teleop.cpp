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

Teleop::Teleop():lifter_goal(Lifter::Goal::stop()),wings_goal(Wings::Goal::LOCKED),collector_mode(Collector_mode::IDLE),started_intake_with_cube(false),high_gear(false)
{
}

void Teleop::runDrivebase(const Run_info &info, Toplevel::Goal &goals)
{
    bool spin=fabs(info.driver_joystick.axis[Gamepad_axis::RIGHTX])>.01;//drive turning button
    double boost=info.driver_joystick.axis[Gamepad_axis::LTRIGGER]; //Turbo button
    bool slow=info.driver_joystick.button[Gamepad_button::LB]; //Slow button
    bool enabled = info.in.robot_mode.enabled;
    
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

void Teleop::runCollector(const Run_info &info, Toplevel::Goal &goals)
{
    paramsInput *params_p = paramsInput::get() ;
    messageLogger &logger = messageLogger::get();
    Lifter::Goal prep_climb_goal = Lifter::Goal::go_to_preset(LifterController::Preset::PREP_CLIMB);
    Lifter::Goal climb_goal = Lifter::Goal::climb();

    //
    // If the grabber tells us we collected a cube, perform teleop cube processing
    //
    if (Grabber::grabber_controller.cubeStateTransition(GrabberController::CubeState::MaybeHasCube, GrabberController::CubeState::HasCube))
    {
		logger << "    Performed acuire cube actions\n" ;
		logger << "    Near preset: " << Lifter::lifter_controller.nearPreset(LifterController::Preset::FLOOR, info.status.lifter.height, 2.0) << "\n";
		logger << "    Is Calibrated: " << Lifter::lifter_controller.isCalibrated() << "\n";
		
		collector_mode = Collector_mode::HOLD_CUBE ;
		double exheight = Lifter::lifter_controller.presetToHeight(LifterController::Preset::EXCHANGE) ;
		if (info.status.lifter.height < exheight && Lifter::lifter_controller.isCalibrated())
		{
			//
			// If we collected the cube witin a small tolerance of the floor height, we move the
			// lifter up to EXCHANGE height.  Note, if we collect the cube at any other height
			// it is already off the floor and we let the drive team deal with height.
			//
			// Also, if the lifter has not been calibrated, we never move the lifter.  We want it
			// to stay on the floor until the lifter is calibrated.
			//
			double delay = params_p->getValue("teleop:collection_delay", 0.5) ;

			//
			// Start a timer for moving to exchange height.  This ensure that is we are collecting
			// from OPEN mode, the grabber arms have time to completely grasp the cube before we
			// start raising the lifter
			//
			collect_delay_timer.set(delay);
			
			logger << "    Started exchange height timer\n" ;
		}
    }
    else if (Grabber::grabber_controller.cubeStateTransition(GrabberController::CubeState::MaybeLostCube, GrabberController::CubeState::NoCube))
    {
		logger << "    Performed lost cube actions\n" ;
		
		//
		// Perform an teleop processing we want where when the grabber tells us we lost a cube
		//
    }

    //
    // Go to exchange height if the timer has expired.  This timer was setup when we saw
    // the transition no cube to having a cube.  Basically it causes us to go to exchange height
    // when we detect a cube present
    //
    collect_delay_timer.update(info.in.now, info.in.robot_mode.enabled);
    if(collect_delay_trigger(collect_delay_timer.done()))
	{
		lifter_goal = Lifter::Goal::go_to_preset(LifterController::Preset::EXCHANGE);
		logger << "    Set lifter goal to exchange height\n" ;
	}

    //
    // Now process the panel inputs and decide what to do.
    //

    //
    // TODO: What is the priority of these operations
    //
    if(collect_open_trigger(info.panel.collect_open))
    {
		if(collector_mode == Collector_mode::COLLECT_OPEN)
		{
			collector_mode = Collector_mode::IDLE;
			logger << "    Collector to IDLE mode\n" ;
		}
		else
		{
			collector_mode = Collector_mode::COLLECT_OPEN;
			logger << "    Collector to OPEN mode\n" ;
		}
    }
    else if(collect_closed_trigger(info.panel.collect_closed))
    {
		if(collector_mode == Collector_mode::COLLECT_CLOSED)
		{
			collector_mode = Collector_mode::IDLE;
			logger << "    Collector to IDLE mode\n" ;
		}
		else
		{
			collector_mode = Collector_mode::COLLECT_CLOSED;
			logger << "    Collector to CLOSED mode\n" ;
		}
    }
    else if(info.panel.eject)
    {
		//
		// BWG: Should these not alse be triggers?
		//
		logger << "    Collector to EJECT\n" ;
		collector_mode = Collector_mode::EJECT;
		started_intake_with_cube = (Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::HasCube) ;
		intake_timer.set(0.1);
    }
    else if(info.panel.drop)
    {
		//
		// BWG: Should these not alse be triggers?
		//
		logger << "    Collector to DROP\n" ;
		collector_mode = Collector_mode::DROP;
		started_intake_with_cube = (Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::HasCube) ;
		intake_timer.set(0.5);
    }
    
    //
    // Process the panel for the lifter commands
    //
    bool prep_climb_done = ready(status(info.status.lifter), prep_climb_goal);
    if(info.panel.climb)
    {
		//
		// The climb buttons means one of two things.  The first time it is pressed
		// prep_climb_done is false and we just move the lifter to climb height.  The
		// second time it is pressed, we actually perform the climb.
		//
		if(!prep_climb_done) {
			logger << "    Climb - moving lifter to climb height\n";
			lifter_goal = prep_climb_goal;
		} else if(info.panel.climb_lock) {
			logger << "    Climb - climbing (climb lock is on)\n";
			lifter_goal = climb_goal ;
		}
    }
    else if(info.panel.floor)
    {
		lifter_goal = Lifter::Goal::go_to_preset(LifterController::Preset::FLOOR);
    }
    else if(info.panel.exchange)
    {
		lifter_goal = Lifter::Goal::go_to_preset(LifterController::Preset::EXCHANGE);
    }
    else if(info.panel.switch_)
    {
		logger << "    requested switch from panel\n" ;
		lifter_goal = Lifter::Goal::go_to_preset(LifterController::Preset::SWITCH);
    }
    else if(info.panel.scale)
    {
		lifter_goal = Lifter::Goal::go_to_preset(LifterController::Preset::SCALE);
    }
    else if(info.panel.lifter == Panel::Lifter::UP)
    {
		lifter_goal = Lifter::Goal::up(info.panel.lifter_high_power);
    }
    else if(info.panel.lifter == Panel::Lifter::DOWN)
    {
		lifter_goal = Lifter::Goal::down(info.panel.lifter_high_power);
    }
	else if(ready(status(info.status.lifter), lifter_goal))
    {
		logger << "    lifter reached goal, stopping\n" ;
		lifter_goal = Lifter::Goal::stop();
    } 
    
    if(lifter_goal == prep_climb_goal && prep_climb_done) {
        logger << "    Climb - shifting to low gear\n" ;
		goals.lifter = Lifter::Goal::low_gear();
    }

    if(info.status.lifter.at_climbed_height){
		goals.lifter = Lifter::Goal::lock(true);
    }	
    
    if(calibrate_lifter_trigger(info.panel.calibrate_lifter)) {
		logger << "    Lifter calibration requested from panel\n" ;
		Lifter::lifter_controller.setCalibrate(true);
		goals.lifter = Lifter::Goal::calibrate();
    }
    
    if(calibrate_grabber_trigger(info.panel.calibrate_grabber)) {
		logger << "    Grabber calibration requested from panel\n" ;
		Grabber::grabber_controller.calibrate() ;
		collector_mode = Collector_mode::CALIBRATE;
    }
    
    if(info.panel.wings && info.panel.climb_lock)
    {
		goals.wings = Wings::Goal::UNLOCKED;
    }
    
    switch(collector_mode) {
    case Collector_mode::IDLE:
		goals.grabber = Grabber::Goal::idle();
		goals.intake = Intake::Goal::off();
		break;
    case Collector_mode::HOLD_CUBE:
		goals.grabber = Grabber::Goal::hold();
		goals.intake = Intake::Goal::off();
		break;
    case Collector_mode::COLLECT_OPEN:
		goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::OPEN);
		goals.intake = Intake::Goal::in();
		if (Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::HasCube)
			collector_mode = Collector_mode::HOLD_CUBE;
		break;
    case Collector_mode::COLLECT_CLOSED:
		goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::CLOSED);
		goals.intake = Intake::Goal::in();
		if (Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::HasCube)
			collector_mode = Collector_mode::HOLD_CUBE;
		break;
    case Collector_mode::EJECT:
    case Collector_mode::DROP:
		if(collector_mode == Collector_mode::EJECT) {
			goals.grabber = Grabber::Goal::hold();
			goals.intake = Intake::Goal::out();
		} else {
			goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::OPEN);
			goals.intake = Intake::Goal::out(0.2);
		}
		intake_timer.update(info.in.now, info.in.robot_mode.enabled);
		if ((started_intake_with_cube && Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::NoCube) || intake_timer.done())
			collector_mode = Collector_mode::IDLE;
		break;
    case Collector_mode::CALIBRATE:
		goals.grabber = Grabber::Goal::calibrate();
		if(ready(info.status.grabber, Grabber::Goal::calibrate()))
			collector_mode = Collector_mode::HOLD_CUBE;
		break;
    default:
		assert(0);
    }

    if(lifter_goal == prep_climb_goal || prep_climb_done) {
		logger << "    Climb - stowing the grabber\n" ;
		goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::STOWED);
    }

    if(!info.panel.grabber_auto) {
		//
		// These override settings made by the collector mode
		//
		if(info.panel.grabber == Panel::Grabber::OFF) goals.grabber = Grabber::Goal::idle();
		if(info.panel.grabber == Panel::Grabber::OPEN) goals.grabber = Grabber::Goal::open();
		if(info.panel.grabber == Panel::Grabber::CLOSE) goals.grabber = Grabber::Goal::close();
    }
    
    if(!info.panel.intake_auto) {
		//
		// These override settings made by the collector mode
		//
		if(info.panel.intake == Panel::Intake::OFF) goals.intake = Intake::Goal::off();
		if(info.panel.intake == Panel::Intake::IN) goals.intake = Intake::Goal::in();
		if(info.panel.intake == Panel::Intake::OUT) goals.intake = Intake::Goal::out();
    }

	goals.lifter = lifter_goal ;
    
    logger << "    Grabber goal: " << goals.grabber << "\n";
	logger << "    Lifter goal: " << goals.lifter.toString() << "\n" ;
    logger << "    Collector mode: " << collector_mode << "\n";
}

void Teleop::runLights(const Run_info &info, Toplevel::Goal &goals)
{
    goals.lights.climbing = goals.lifter.preset_target() == LifterController::Preset::PREP_CLIMB;
    goals.lights.lifter_height = (unsigned)info.status.lifter.height;
    goals.lights.drive_left = goals.drive.left();
    goals.lights.drive_right = goals.drive.right();
    goals.lights.has_cube = (Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::HasCube) ;

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

Toplevel::Goal Teleop::run(Run_info info)
{
    messageLogger &logger = messageLogger::get();
    logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_TELEOP);
    logger << "Teleop:\n";
    
    Toplevel::Goal goals;
    
    runDrivebase(info, goals) ;
    runCollector(info, goals) ;
    runLights(info, goals) ;

    logger.endMessage() ;

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
