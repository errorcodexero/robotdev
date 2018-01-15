#include "step.h"
#include<queue>
#include "executive.h"
#include "../util/util.h"
#include <cmath>
//#include "teleop.h"

using namespace std;

//this part should eventually go into util
template<typename T>
queue<T>& operator|=(queue<T> &q,T t){
	q.push(t);
	return q;
}

ostream& operator<<(ostream& o,Step const& a){
	a.display(o);
	return o;
}

//This part stays in the CPP file.
Toplevel::Goal Step::run(Run_info info, Toplevel::Goal goals){
	return impl->run(info,goals);
}
Toplevel::Goal Step::run(Run_info info){
	return impl->run(info,{});
}

const double RIGHT_SPEED_CORRECTION = /*-0.045; */ 0.0;// 0 is for comp bot. //left and right sides of the practice robot drive at different speeds given the same power, adjust this to make the robot drive straight

Drivebase::Distances Rotate::angle_to_distances(Rad target_angle){
	Inch side_goal = target_angle * 0.5 * Robot_constants::ROBOT_WIDTH;
	return Drivebase::Distances{side_goal,-side_goal};
}

Drivebase::Distances Rotate::get_distance_travelled(Drivebase::Distances current){
	return current - initial_distances;
}

Rotate::Rotate(Rad a):Rotate(a,0.02,0.5){}//from testing
Rotate::Rotate(Rad a,double vel_modifier,double max):target_angle(a),initial_distances({0,0}),init(false),side_goals(angle_to_distances(a)){
	motion_profile = {side_goals.l,vel_modifier,max};//from testing
}

Toplevel::Goal Rotate::run(Run_info info){
	return run(info,{});
}

Toplevel::Goal Rotate::run(Run_info info,Toplevel::Goal goals){
	if(!init){
		initial_distances = info.status.drive.distances;
		init = true;
	}
	Drivebase::Distances distance_travelled = get_distance_travelled(info.status.drive.distances);
	
	//ignoring right encoder because it's proven hard to get meaningful data from it
	double power = motion_profile.target_speed(distance_travelled.l); 
	double left = clip(target_to_out_power(power));//TODO: move .2 to the constructor of Rotate and set an instance variable
	double right = -clip(target_to_out_power(power - RIGHT_SPEED_CORRECTION * power));
	goals.drive = Drivebase::Goal::absolute(left,right);
	return goals;
}

Step::Status Rotate::done(Next_mode_info info){
	static const Inch TOLERANCE = 1.0;//inches
	Drivebase::Distances distance_travelled = get_distance_travelled(info.status.drive.distances);
	Drivebase::Distances distance_left = side_goals - distance_travelled;
	if(fabs(distance_left.l) < TOLERANCE){
		 in_range.update(info.in.now,info.in.robot_mode.enabled);
	} else {
		static const Time FINISH_TIME = .50;//seconds
		in_range.set(FINISH_TIME);
	}
	return in_range.done() ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
}

std::unique_ptr<Step_impl> Rotate::clone()const{
	return unique_ptr<Step_impl>(new Rotate(*this));
}

bool Rotate::operator==(Rotate const& b)const{
	return target_angle == b.target_angle && initial_distances == b.initial_distances && side_goals == b.side_goals && motion_profile == b.motion_profile && in_range == b.in_range;
}
////

Navx_rotate::Navx_rotate(double a):target_angle(a){}

Toplevel::Goal Navx_rotate::run(Run_info info){
	return run(info,{});
}

Toplevel::Goal Navx_rotate::run(Run_info info,Toplevel::Goal goals){
	drive_goal = Drivebase::Goal::rotate(info.status.drive.angle + target_angle);
	goals.drive = *drive_goal;
	return goals;
}

Step::Status Navx_rotate::done(Next_mode_info info){
	drive_goal = Drivebase::Goal::rotate(info.status.drive.angle + target_angle);
	return ready(info.status.drive, *drive_goal) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;	
}

std::unique_ptr<Step_impl> Navx_rotate::clone()const{
	return unique_ptr<Step_impl>(new Navx_rotate(*this));
}

bool Navx_rotate::operator==(Navx_rotate const& b)const{
	return target_angle == b.target_angle;
}

//Step::Step(Step_impl const& a):impl(a.clone().get()){}
Step::Step(Step_impl const& a){
	auto c=a.clone();
	if(!c)nyi
	impl=move(c);
}

Step::Status Step::done(Next_mode_info a){
	return impl->done(a);
}

void Step::display(ostream& o)const{
	impl->display(o);
}

bool Step::operator==(Step const& a)const{
	assert(impl);
	return impl->operator==(a);
}

bool Step::operator<(Step const& a)const{
	assert(impl);
	return impl->operator<(a);
}

void Step_impl::display(ostream& o)const{
	o<<"Step_impl";
}

Step_impl::~Step_impl(){}

/*bool Step_impl::operator==(Step_impl const& a)const{
	T const& b=dynamic_cast<T const&>(a.get());
	return this->operator==(b);
}*/

Drive::Drive(double t){
	timer.set(t);
}

Step::Status Drive::done(Next_mode_info /*info*/){
	return timer.done() ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
}

Toplevel::Goal Drive::run(Run_info info){
	return run(info,{});
}

Toplevel::Goal Drive::run(Run_info info,Toplevel::Goal goals){
	timer.update(info.in.now, info.in.robot_mode.enabled);
	const double POWER = 0.4;
	goals.drive = Drivebase::Goal::absolute(POWER,POWER);
	return goals;
}

unique_ptr<Step_impl> Drive::clone()const{
	return unique_ptr<Step_impl>(new Drive(*this));
}

bool Drive::operator==(Drive const& b)const{
	return timer == b.timer;
}

Drive_straight::Drive_straight(Inch goal):Drive_straight(goal,0.02,0.5){}
Drive_straight::Drive_straight(Inch goal,double vel_modifier,double max):target_dist(goal),initial_distances(Drivebase::Distances{0,0}),init(false),motion_profile(goal,vel_modifier,max){}//Motion profiling values from testing

Drivebase::Distances Drive_straight::get_distance_travelled(Drivebase::Distances current){
	return current - initial_distances;
}

Step::Status Drive_straight::done(Next_mode_info info){
	static const Inch TOLERANCE = 3.0;//inches
	Drivebase::Distances distance_travelled = get_distance_travelled(info.status.drive.distances);
	Drivebase::Distances distance_left = Drivebase::Distances{target_dist,target_dist} - distance_travelled;
	//ignoring right encoder because it's proven hard to get meaningful data from it
	if(fabs(distance_left.l) < TOLERANCE){
		in_range.update(info.in.now,info.in.robot_mode.enabled);
	} else {
		static const Time FINISH_TIME = .50;
		in_range.set(FINISH_TIME);
	}
	
	/*
	if(info.status.drive.stall){
		stall_timer.update(info.in.now,info.in.robot_mode.enabled);
	} else{
		static const Time STALL_TIME = 1.0;
		stall_timer.set(STALL_TIME);
	}
	if(stall_timer.done()) return Step::Status::FINISHED_FAILURE;
	*/
	
	//cout<<"stall:"<<info.status.drive.stall<<"\n";

	return in_range.done() ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
}

Toplevel::Goal Drive_straight::run(Run_info info){
	return run(info,{});
}

Toplevel::Goal Drive_straight::run(Run_info info,Toplevel::Goal goals){
	if(!init){
		initial_distances = info.status.drive.distances;
		init = true;
	}
	Drivebase::Distances distance_travelled = get_distance_travelled(info.status.drive.distances);

	double power = motion_profile.target_speed(distance_travelled.l); //ignoring right encoder because it's proven hard to get meaningful data from it

	double left = clip(target_to_out_power(power));//TODO: move .11 to the constructor of Drive_straight and set an instance variable
	double right = clip(target_to_out_power(power + power * RIGHT_SPEED_CORRECTION)); //right side would go faster than the left without error correction
	goals.drive = Drivebase::Goal::absolute(left,right);
	return goals;
}

unique_ptr<Step_impl> Drive_straight::clone()const{
	return unique_ptr<Step_impl>(new Drive_straight(*this));
}

bool Drive_straight::operator==(Drive_straight const& b)const{
	return target_dist == b.target_dist && initial_distances == b.initial_distances && init == b.init && motion_profile == b.motion_profile && in_range == b.in_range /*&& stall_timer == b.stall_timer*/;
}

MP_drive::MP_drive(Inch target):target_distance(target){}

Step::Status MP_drive::done(Next_mode_info info){
	drive_goal = Drivebase::Goal::distances(Drivebase::Distances(target_distance) + info.status.drive.distances);
	return ready(info.status.drive, *drive_goal) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
}

Toplevel::Goal MP_drive::run(Run_info info){
	return run(info, {});
}

Toplevel::Goal MP_drive::run(Run_info info, Toplevel::Goal goals){
	drive_goal = Drivebase::Goal::distances(Drivebase::Distances(target_distance) + info.status.drive.distances);
	goals.drive = *drive_goal;
	return goals;
}

unique_ptr<Step_impl> MP_drive::clone()const{
	return unique_ptr<Step_impl>(new MP_drive(*this));
}

bool MP_drive::operator==(MP_drive const& a)const{
	return target_distance==a.target_distance && drive_goal==a.drive_goal;
}

Navx_drive_straight::Navx_drive_straight(Inch target):target_distance(target),angle_i(0){}

Step::Status Navx_drive_straight::done(Next_mode_info info){
	drive_goal = Drivebase::Goal::drive_straight(Drivebase::Distances(target_distance) + info.status.drive.distances, info.status.drive.angle, angle_i);
	return ready(info.status.drive, *drive_goal) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
}

Toplevel::Goal Navx_drive_straight::run(Run_info info){
	return run(info, {});
}

Toplevel::Goal Navx_drive_straight::run(Run_info info, Toplevel::Goal goals){
	drive_goal = Drivebase::Goal::drive_straight(Drivebase::Distances(target_distance) + info.status.drive.distances, info.status.drive.angle, angle_i);
	angle_i += (total_angle_to_displacement((*drive_goal).angle()) - total_angle_to_displacement(info.status.drive.angle)) * info.status.drive.dt;
	goals.drive = Drivebase::Goal::drive_straight((*drive_goal).distances(), (*drive_goal).angle(), angle_i);
	return goals;
}

unique_ptr<Step_impl> Navx_drive_straight::clone()const{
	return unique_ptr<Step_impl>(new Navx_drive_straight(*this));
}

bool Navx_drive_straight::operator==(Navx_drive_straight const& a)const{
	return target_distance==a.target_distance && drive_goal==a.drive_goal;
}

Ram::Ram(Inch goal):target_dist(goal),initial_distances(Drivebase::Distances{0,0}),init(false){}

Drivebase::Distances Ram::get_distance_travelled(Drivebase::Distances current){
	return current - initial_distances;
}

Step::Status Ram::done(Next_mode_info info){
	static const Inch TOLERANCE = 0.0;//inches
	Drivebase::Distances distance_travelled = get_distance_travelled(info.status.drive.distances);
	//Drivebase::Distances distance_left = Drivebase::Distances{target_dist,target_dist} - distance_travelled;
	//ignoring right encoder because it's proven hard to get meaningful data from it
	return (fabs(distance_travelled.l) >= fabs(target_dist) - TOLERANCE) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
}

Toplevel::Goal Ram::run(Run_info info){
	return run(info,{});
}

Toplevel::Goal Ram::run(Run_info info,Toplevel::Goal goals){
	if(!init){
		initial_distances = info.status.drive.distances;
		init = true;
	}

	static const double POWER = .5;
	double p = copysign(POWER,target_dist);
	
	{
		double left = p;
		double right = p + p * RIGHT_SPEED_CORRECTION; //right side would go faster than the left without error correction
		goals.drive = Drivebase::Goal::absolute(left,right);
	}
	return goals;
}

unique_ptr<Step_impl> Ram::clone()const{
	return unique_ptr<Step_impl>(new Ram(*this));
}

bool Ram::operator==(Ram const& b)const{
	return target_dist == b.target_dist && initial_distances == b.initial_distances && init == b.init /*&& stall_timer == b.stall_timer*/;
}

Wait::Wait(Time wait_time){
	wait_timer.set(wait_time);
}

Step::Status Wait::done(Next_mode_info){
	return wait_timer.done() ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
}

Toplevel::Goal Wait::run(Run_info info){
	return run(info,{});
}

Toplevel::Goal Wait::run(Run_info info,Toplevel::Goal goals){
	wait_timer.update(info.in.now,info.in.robot_mode.enabled);
	return goals;
}

unique_ptr<Step_impl> Wait::clone()const{
	return unique_ptr<Step_impl>(new Wait(*this));
}

bool Wait::operator==(Wait const& b)const{
	return wait_timer == b.wait_timer;
}

void Combo::display(std::ostream& o)const{
	Step_impl_inner<Combo>::display(o);
	o<<"(";
	step_a.display(o);
	o<<" ";
	step_b.display(o);
	o<<")";
}

Combo::Combo(Step a,Step b):step_a(a),step_b(b){}

Step::Status Combo::done(Next_mode_info info){
	Step::Status a_status = step_a.done(info);
	Step::Status b_status = step_b.done(info);
	switch(a_status){
		case Step::Status::FINISHED_SUCCESS:
			return b_status;
		case Step::Status::UNFINISHED:
			return a_status;//TODO
		case Step::Status::FINISHED_FAILURE:
			nyi //TODO
		default:
			assert(0);
	} 
}

Toplevel::Goal Combo::run(Run_info info){
	return run(info,{});
}

Toplevel::Goal Combo::run(Run_info info,Toplevel::Goal goals){
	goals = step_a.run(info,goals);
	goals = step_b.run(info,goals);
	return goals;
}

unique_ptr<Step_impl> Combo::clone()const{
	return unique_ptr<Step_impl>(new Combo(*this));
}

bool Combo::operator==(Combo const& b)const{
	return step_a == b.step_a && step_b == b.step_b;
}

Step_impl const& Step::get()const{
	assert(impl);
	return *impl.get();
}

Step::Step(Step const& a):impl(a.get().clone()){}

/*
to put in its own file:

*/
class Do_list:public Step_impl_inner<Do_list>{
	vector<Step> steps;
	size_t index;

	public:
	explicit Do_list(vector<Step>);

	virtual Toplevel::Goal run(Run_info,Toplevel::Goal);
	virtual Toplevel::Goal run(Run_info);
	virtual Step::Status done(Next_mode_info);
	//virtual unique_ptr<Step_impl> clone()const;
	//virtual void display(ostream&)const;
	bool operator==(Do_list const&)const;
};

Do_list::Do_list(vector<Step> v):steps(v),index(0){}

Toplevel::Goal Do_list::run(Run_info info){
	return run(info,{});
}

Toplevel::Goal Do_list::run(Run_info,Toplevel::Goal){
	nyi
}

bool Do_list::operator==(Do_list const& a)const{
	return steps==a.steps && index==a.index;
}

Step::Status Do_list::done(Next_mode_info){
	nyi
}

ostream& operator<<(ostream& o,Step_impl const& a){
	a.display(o);
	return o;
}
#ifdef STEP_TEST
void test_step(Step a){
	PRINT(a);
	auto b=a;
	assert(b==a);
	//see that can at least run "run", and "done"
	//a.run(example
	//TODO: try to visit all the possible states that you can get to from the initial one, and see that things don't fail
	set<Step> visited;
	queue<Step> to_visit;
	to_visit|=a;

	//might want to put some limit on the amount of state space that's used.
	//TODO: make a list of the inputs to try
	//first, run all the "run"s
	//second, run ready on all the different things and see if any of them change
	//abort if they do change

	while(to_visit.size()){
		auto f=to_visit.front();
		to_visit.pop();

		if(visited.find(f)!=visited.end()) continue;

		visited|=f;

		//TODO: Actually run some data on it.
	}

	//see that will at some point "done" can be set
}

template<typename T>
void test_step(T t){
	return test_step(Step{t});
}

int main(){
	/*Drive_straight a;
	Chain b{Step{a},Executive{Teleop{}}};
	Chain c{b};
	auto d=c.next_mode(example((Next_mode_info*)0));
	PRINT(c.step);
	PRINT(d);*/

	Do_list dl{{}};
	//PRINT(dl);
	test_step(dl);
}
#endif
