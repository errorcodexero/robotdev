#include "chain.h"
#include "teleop.h"

using namespace std;

Chain::Chain(vector<Step>& s, Executive n):current_step(0),steps(&s),next(n)
{
}

Toplevel::Goal Chain::run(Run_info info)
{
	if(current_step>=steps->size()) return Toplevel::Goal();
	return (*steps)[current_step].run(info);
}

Executive Chain::next_mode(Next_mode_info a)
{
	if(!a.autonomous)
		return Executive{Teleop()};

	if(current_step >= steps->size())
		return next;

	Step &current = (*steps)[current_step] ;
	switch(current.done(a))
	{
		case Step::Status::FINISHED_SUCCESS:
			current.finishStep(a.in.now) ;
			current_step++;
			if(current_step==steps->size())
				return next;

			//
			// Initialize the next step
			//
			(*steps)[current_step].startStep(a.in.now) ;
			(*steps)[current_step].init(a.in, a.status) ;
			return Executive{*this};
		case Step::Status::FINISHED_FAILURE:
			return Executive{Teleop()} ;
			break ;
		case Step::Status::UNFINISHED:
			return Executive{*this};
		default:
			assert(0);
	}
}

void Chain::display(ostream& o)const{
	o<<"Chain(";
	o<<"current_step:"<<current_step<<" steps:"<<*steps;
	o<<")";
}

bool Chain::operator==(Chain const& b)const{
	return current_step == b.current_step && steps == b.steps && next == b.next;
}

bool Chain::operator<(Chain const& b)const{
	return current_step < b.current_step;
}

unique_ptr<Executive_interface> Chain::clone()const{
	return unique_ptr<Executive_interface>(new Chain(*this));
	//return make_unique<Chain>(step);
	//assert(0);
}

#ifdef CHAIN_TEST
int main(){
	Drive_straight a{24};
	Drive_straight b{-24};
	vector<Step> steps={Step{a},Step{b}};
	Chain c{steps,Executive{Teleop{}}};
	Chain d{c};
	auto e=d.next_mode(example((Next_mode_info*)0));
	PRINT(d.steps);
	PRINT(e);
	/*auto f=d.run(example((Run_info*)0));
	PRINT(f);
	d.current_step++;
	auto g=d.run(example((Run_info*)0));
	PRINT(g);*/
	//test_executive(Chain(Step(),Executive()));
}
#endif
