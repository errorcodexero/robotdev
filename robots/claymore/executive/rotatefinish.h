#include "step.h"

//Rotate the robot by a specified angle
struct Rotate_finish: Step_impl_inner<Rotate_finish>{
    double target_angle;
	double prev_angle ;
	double tolerance ;
    bool init;
	bool tolprovided ;

    explicit Rotate_finish(double prevangle, double angle);
    explicit Rotate_finish(double prevangle, double angle, double tol);
	
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Rotate_finish const&)const;
};

