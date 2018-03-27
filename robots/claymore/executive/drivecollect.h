#include "step.h"

//Drive forward and collect until a cube is collected
struct Drive_and_collect: Step_impl_inner<Drive_and_collect>{
	static double distance_travelled;

	Countdown_timer timeout_timer;
	Drivebase::Distances initial_distances;
	bool init;
	double mStart ;
	double maxdistance ;

	explicit Drive_and_collect();
	explicit Drive_and_collect(double maxdist) ;

	Toplevel::Goal run(Run_info,Toplevel::Goal);
	Toplevel::Goal run(Run_info);
	Step::Status done(Next_mode_info);
	std::unique_ptr<Step_impl> clone()const;
	bool operator==(Drive_and_collect const&)const;
};

