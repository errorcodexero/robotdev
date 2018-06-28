#include "teleop.h"
#include <math.h>
#include "autonomous.h"
#include "iutil.h"

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

Executive Teleop::next_mode(Next_mode_info info) {
	if (info.autonomous_start) {
		return Executive{Autonomous()};
	}
	//Teleop t(CONSTRUCT_STRUCT_PARAMS(TELEOP_ITEMS));
	return Executive{*this};
}

IMPL_STRUCT(Teleop::Teleop,TELEOP_ITEMS)

Teleop::Teleop():print_number(0){}

Toplevel::Goal Teleop::run(Run_info info) {
	Toplevel::Goal goals;
	
	bool enabled = info.in.robot_mode.enabled;

	{//Set drive goals
		double boost=info.driver_joystick.axis[Gamepad_axis::LTRIGGER]; //Turbo button
		bool slow=info.driver_joystick.button[Gamepad_button::LB]; //Slow button

		POV_section driver_pov=pov_section(info.driver_joystick.pov);
		
		const array<POV_section,4> nudge_povs={POV_section::UP,POV_section::DOWN,POV_section::LEFT,POV_section::RIGHT};
		//Forward, backward, left, right
		for(unsigned i=0;i<NUDGES;i++){
			bool nudge_activated;
			if(i < 4)
				nudge_activated = driver_pov == nudge_povs[i];
			else if(i == 4)
				nudge_activated = info.driver_joystick.button[Gamepad_button::B];
			else
				nudge_activated = info.driver_joystick.button[Gamepad_button::X];

			if(nudges[i].trigger(boost<.25 && nudge_activated)) nudges[i].timer.set(.1);
			nudges[i].timer.update(info.in.now,enabled);
		}

		const double NUDGE_POWER=.2,ROTATE_NUDGE_POWER=.2;
		goals.drive.x = clip([&]{
			if (!nudges[Nudges::LEFT].timer.done()) return NUDGE_POWER;
			if (!nudges[Nudges::RIGHT].timer.done()) return -NUDGE_POWER;
			return set_drive_speed(info.driver_joystick.axis[Gamepad_axis::LEFTX],boost,slow);
		}());
		goals.drive.y = clip([&]{
			if (!nudges[Nudges::FORWARD].timer.done()) return NUDGE_POWER;
			if (!nudges[Nudges::BACKWARD].timer.done()) return -NUDGE_POWER;
			return set_drive_speed(info.driver_joystick.axis[Gamepad_axis::LEFTY],boost,slow);
		}());
		goals.drive.theta = clip([&]{ // - = cw, + = ccw
			if (!nudges[Nudges::CLOCKWISE].timer.done()) return -ROTATE_NUDGE_POWER;
			if (!nudges[Nudges::COUNTERCLOCKWISE].timer.done()) return ROTATE_NUDGE_POWER;
			return set_drive_speed(-info.driver_joystick.axis[Gamepad_axis::RIGHTX],boost,slow);
		}());
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
