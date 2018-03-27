#include "step.h"
#include "executive.h"
#include "util.h"
#include "message_logger.h"
#include "params_parser.h"
#include "../subsystems.h"
#include <queue>
#include <cmath>


using namespace std;

//
// Combo: Run two steps simultaneously
//

Combo::Combo(Step &a,Step &b):Step("combo"), step_a(a),step_b(b)
{
}

Combo::Combo(const char *name_p, Step &a,Step &b):Step(name_p), step_a(a),step_b(b)
{
}

void Combo::init(const Robot_inputs &in, Toplevel::Status_detail &status)
{
	step_a.init(in, status) ;
	step_b.init(in, status) ;
}

Step::Status Combo::done(Next_mode_info info)
{
    Step::Status a_status = step_a.done(info);
    Step::Status b_status = step_b.done(info);
	
    switch(a_status)
	{
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

Toplevel::Goal Combo::run(Run_info info)
{
    return run(info,{});
}

Toplevel::Goal Combo::run(Run_info info,Toplevel::Goal goals)
{
    goals = step_a.run(info,goals);
    goals = step_b.run(info,goals);
    return goals;
}

////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
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
