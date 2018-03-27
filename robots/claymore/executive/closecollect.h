#include "step.h"

struct Close_collect: Step_impl_inner<Close_collect>{
	bool mInit ;
	double mTime ;
	double mStart ;
	Countdown_timer timeout_timer;
	
	explicit Close_collect(double length);

	Toplevel::Goal run(Run_info,Toplevel::Goal);
	Toplevel::Goal run(Run_info);
	Step::Status done(Next_mode_info);
	std::unique_ptr<Step_impl> clone()const;
	bool operator==(Close_collect const&)const;
};
