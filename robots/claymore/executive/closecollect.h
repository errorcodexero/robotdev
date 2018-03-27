#include "step.h"

class Close_collect: Step
{
private:
	bool mInit ;
	double mTime ;
	double mStart ;
	Countdown_timer timeout_timer;

public:
	explicit Close_collect(double length);

	Toplevel::Goal run(Run_info,Toplevel::Goal);
	Toplevel::Goal run(Run_info);
	Step::Status done(Next_mode_info);
};
