#include "step.h"

//Drive forward and collect until a cube is collected
class Drive_and_collect: public Step
{
public:
	static double distance_travelled;

private:
	Countdown_timer timeout_timer;
	Drivebase::Distances initial_distances;
	double mStart ;
	double maxdistance ;

public:
	explicit Drive_and_collect();
	explicit Drive_and_collect(double maxdist) ;

	void init(const Robot_inputs &in, Toplevel::Status_detail &status);
	Toplevel::Goal run(Run_info,Toplevel::Goal);
	Toplevel::Goal run(Run_info);
	Step::Status done(Next_mode_info);
};

