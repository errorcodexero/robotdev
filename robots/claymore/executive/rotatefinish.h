#include "step.h"

//Rotate the robot by a specified angle
class Rotate_finish: Step
{
private:
    double target_angle;
	double prev_angle ;
	double tolerance ;
	bool tolprovided ;

public:
    explicit Rotate_finish(double prevangle, double angle);
    explicit Rotate_finish(double prevangle, double angle, double tol);
	
	void init(const Robot_inputs &in, Toplevel::Status_detail &status) ;
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
};

