#include "step.h"
#include<queue>
#include "executive.h"
#include "util.h"
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
//


Step::Step(Step const& a):impl(a.get().clone()){}

Step::Step(Step_impl const& a){
	auto c=a.clone();
	if(!c)nyi
	impl=move(c);
}

Toplevel::Goal Step::run(Run_info info, Toplevel::Goal goals){
	return impl->run(info,goals);
}
Toplevel::Goal Step::run(Run_info info){
	return impl->run(info,{});
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

Step_impl const& Step::get()const{
	assert(impl);
	return *impl.get();
}

ostream& operator<<(ostream& o,Step_impl const& a){
	a.display(o);
	return o;
}

/*bool Step_impl::operator==(Step_impl const& a)const{
	T const& b=dynamic_cast<T const&>(a.get());
	return this->operator==(b);
}*/

Step_impl::~Step_impl(){}


//
// Combo: Run two steps simultaneously
//

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

//
// Wait: Wait for a specified amount of time
//

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

//
// Drive: Drive straight a specified distance
//

Drive::Drive(Inch target):target_distance(target),init(false){}

Step::Status Drive::done(Next_mode_info info){
	return ready(info.status.drive, Drivebase::Goal::drive_straight()) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
}

Toplevel::Goal Drive::run(Run_info info){
	return run(info, {});
}

Toplevel::Goal Drive::run(Run_info info, Toplevel::Goal goals){
	if(!init) {
		double avg_status = (info.status.drive.distances.l + info.status.drive.distances.r) / 2.0;
		Drivebase::drivebase_controller.initDistance(avg_status + target_distance, info.status.drive.angle, info.in.now);
		init = true;
	}
	goals.drive = Drivebase::Goal::drive_straight();
	return goals;
}

unique_ptr<Step_impl> Drive::clone()const{
	return unique_ptr<Step_impl>(new Drive(*this));
}

bool Drive::operator==(Drive const& a)const{
	return target_distance == a.target_distance && init == a.init;
}

//
// Drive_timed: Drive the motors at the specified powers for a specified amount of time
//

Drive_timed::Drive_timed(double l, double r, double t){
	left_power = l;
	right_power = r;
	timer.set(t);
}

Step::Status Drive_timed::done(Next_mode_info /*info*/){
	return timer.done() ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
}

Toplevel::Goal Drive_timed::run(Run_info info){
	return run(info,{});
}

Toplevel::Goal Drive_timed::run(Run_info info,Toplevel::Goal goals){
	timer.update(info.in.now, info.in.robot_mode.enabled);
	goals.drive = Drivebase::Goal::absolute(left_power, right_power);
	return goals;
}

unique_ptr<Step_impl> Drive_timed::clone()const{
	return unique_ptr<Step_impl>(new Drive_timed(*this));
}

bool Drive_timed::operator==(Drive_timed const& b)const{
	return left_power == b.left_power && right_power == b.right_power && timer == b.timer;
}

//
// Ram: Drive the motors at the specified powers for the specified distances
//

Ram::Ram(double l_power, double r_power, Inch l_target, Inch r_target){
	left_power = l_power;
	right_power = r_power;
	target_distances = Drivebase::Distances(l_target, r_target);
	initial_distances = Drivebase::Distances(0.0, 0.0);
	init = false;
}

Step::Status Ram::done(Next_mode_info info){
	bool left_reached = (target_distances.l > initial_distances.l) ? (target_distances.l < info.status.drive.distances.l) : (target_distances.l > info.status.drive.distances.l);
	bool right_reached = (target_distances.r > initial_distances.r) ? (target_distances.r < info.status.drive.distances.r) : (target_distances.r > info.status.drive.distances.r);
	return (init && left_reached && right_reached) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
}

Toplevel::Goal Ram::run(Run_info info){
	return run(info,{});
}

Toplevel::Goal Ram::run(Run_info info,Toplevel::Goal goals){
	if(!init){
		initial_distances = info.status.drive.distances;
		target_distances = target_distances + initial_distances;
		init = true;
	}

	goals.drive = Drivebase::Goal::absolute(left_power, right_power);
	return goals;
}

unique_ptr<Step_impl> Ram::clone()const{
	return unique_ptr<Step_impl>(new Ram(*this));
}

bool Ram::operator==(Ram const& b)const{
	return left_power == b.left_power && right_power == b.right_power && target_distances == b.target_distances && initial_distances == b.initial_distances && init == b.init;
}

//
// Rotate: Rotate the robot by a specified angle
//

Rotate::Rotate(double a):target_angle(a),init(false){}

Toplevel::Goal Rotate::run(Run_info info){
	return run(info,{});
}

Toplevel::Goal Rotate::run(Run_info info,Toplevel::Goal goals){
	if(!init) {
		Drivebase::drivebase_controller.initAngle(info.status.drive.angle + target_angle, info.in.now) ;
		init = true;
	}

	goals.drive = Drivebase::Goal::rotate();
	return goals;
}

Step::Status Rotate::done(Next_mode_info info){
	return ready(info.status.drive, Drivebase::Goal::rotate()) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;	
}

std::unique_ptr<Step_impl> Rotate::clone()const{
	return unique_ptr<Step_impl>(new Rotate(*this));
}

bool Rotate::operator==(Rotate const& b)const{
	return target_angle == b.target_angle && init == b.init;
}

//
// Start_lifter_in_background: Start moving the lifter to a specified preset in the background
//

Start_lifter_in_background::Start_lifter_in_background(LifterController::Preset preset, double time):preset(preset),time(time),init(false){}

Step::Status Start_lifter_in_background::done(Next_mode_info){
	return Step::Status::FINISHED_SUCCESS;
}

Toplevel::Goal Start_lifter_in_background::run(Run_info info){
        return run(info,{});
}

Toplevel::Goal Start_lifter_in_background::run(Run_info info,Toplevel::Goal goals){
	if(!init) {
		Lifter::lifter_controller.backgroundMoveToHeight(preset, info.status.lifter.height, time);
		init = false;
	}
        return goals;
}

unique_ptr<Step_impl> Start_lifter_in_background::clone()const{
        return unique_ptr<Step_impl>(new Start_lifter_in_background(*this));
}

bool Start_lifter_in_background::operator==(Start_lifter_in_background const& b)const{
	return preset == b.preset && time == b.time && init == b.init;
}

//
// Wait_for_lifter: Wait until the lifter has reached its goal
//

Wait_for_lifter::Wait_for_lifter(){}

Step::Status Wait_for_lifter::done(Next_mode_info info){
        return ready(status(info.status.lifter), Lifter::Goal::background()) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
}

Toplevel::Goal Wait_for_lifter::run(Run_info info){
        return run(info,{});
}

Toplevel::Goal Wait_for_lifter::run(Run_info info,Toplevel::Goal goals){
	goals.lifter = Lifter::Goal::background();
        return goals;
}

unique_ptr<Step_impl> Wait_for_lifter::clone()const{
        return unique_ptr<Step_impl>(new Wait_for_lifter(*this));
}

bool Wait_for_lifter::operator==(Wait_for_lifter const& b)const{
        return true;
}

//
// Calibrate_lifter: Calibrate the lifter at the current height
//

Calibrate_lifter::Calibrate_lifter(){
	Lifter::lifter_controller.calibrate();
}

Step::Status Calibrate_lifter::done(Next_mode_info info){
	return ready(status(info.status.lifter), Lifter::Goal::calibrate()) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
}

Toplevel::Goal Calibrate_lifter::run(Run_info info){
	return run(info,{});
}

Toplevel::Goal Calibrate_lifter::run(Run_info info,Toplevel::Goal goals){
	goals.lifter = Lifter::Goal::calibrate();
	return goals;
}

unique_ptr<Step_impl> Calibrate_lifter::clone()const{
	return unique_ptr<Step_impl>(new Calibrate_lifter(*this));
}

bool Calibrate_lifter::operator==(Calibrate_lifter const& b)const{
	return true;
}

//
// Lifter_to_height: Move the lifter to a specified height
//

Lifter_to_height::Lifter_to_height(double target_height, double time):target_height(target_height),time(time),init(false){}

Step::Status Lifter_to_height::done(Next_mode_info info){
	return ready(status(info.status.lifter), Lifter::Goal::go_to_height(target_height)) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
}

Toplevel::Goal Lifter_to_height::run(Run_info info){
	return run(info,{});
}

Toplevel::Goal Lifter_to_height::run(Run_info info,Toplevel::Goal goals){
	if(!init) {
		Lifter::lifter_controller.moveToHeight(target_height, info.status.lifter.height, time);
		init = false;
	}
	goals.lifter = Lifter::Goal::go_to_height(target_height);
	return goals;
}

unique_ptr<Step_impl> Lifter_to_height::clone()const{
	return unique_ptr<Step_impl>(new Lifter_to_height(*this));
}

bool Lifter_to_height::operator==(Lifter_to_height const& b)const{
	return target_height == b.target_height && time == b.time && init == b.init;
}

//
// Calibrate_grabber: Calibrate the grabber at the current angle
//

Calibrate_grabber::Calibrate_grabber(){
	Grabber::grabber_controller.setDoneCalibrating(false);
}

Step::Status Calibrate_grabber::done(Next_mode_info info){
	return ready(status(info.status.grabber), Grabber::Goal::calibrate()) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
}

Toplevel::Goal Calibrate_grabber::run(Run_info info){
	return run(info,{});
}

Toplevel::Goal Calibrate_grabber::run(Run_info info,Toplevel::Goal goals){
	goals.grabber = Grabber::Goal::calibrate();
	return goals;
}

unique_ptr<Step_impl> Calibrate_grabber::clone()const{
	return unique_ptr<Step_impl>(new Calibrate_grabber(*this));
}

bool Calibrate_grabber::operator==(Calibrate_grabber const& b)const{
	return true;
}

//
// Grabber_to_preset: Move the grabber to a specified angle preset
//

Grabber_to_preset::Grabber_to_preset(GrabberController::Preset target_preset, double time):target_preset(target_preset),time(time),init(false){}

Step::Status Grabber_to_preset::done(Next_mode_info info){
        return ready(status(info.status.grabber), Grabber::Goal::go_to_preset(target_preset)) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
}

Toplevel::Goal Grabber_to_preset::run(Run_info info){
        return run(info,{});
}

Toplevel::Goal Grabber_to_preset::run(Run_info info,Toplevel::Goal goals){
        if(!init) {
                Grabber::grabber_controller.moveToAngle(target_preset, time);
                init = false;
        }
        goals.grabber = Grabber::Goal::go_to_preset(target_preset);
        return goals;
}

unique_ptr<Step_impl> Grabber_to_preset::clone()const{
        return unique_ptr<Step_impl>(new Grabber_to_preset(*this));
}

bool Grabber_to_preset::operator==(Grabber_to_preset const& b)const{
        return target_preset == b.target_preset && time == b.time && init == b.init;
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
}
#endif
