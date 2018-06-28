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

//This part stays in the CPP file.
Toplevel::Goal Step::run(Run_info info, Toplevel::Goal goals){
	return impl->run(info,goals);
}
Toplevel::Goal Step::run(Run_info info){
	return impl->run(info,{});
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
