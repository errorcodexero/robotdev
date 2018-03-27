#include "step.h"

//Rotate the robot by a specified angle
struct Rotate: Step_impl_inner<Rotate>{
    double target_angle;
    bool init;
	double tolerance ;
	bool tolprovided ;

    explicit Rotate(double angle);
	explicit Rotate(double angle, double tolerance) ;
	
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Rotate const&)const;
};
