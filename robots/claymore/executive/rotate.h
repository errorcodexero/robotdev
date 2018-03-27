#include "step.h"

//Rotate the robot by a specified angle
class Rotate: public Step
{
private:
    double target_angle;
	double tolerance ;
	bool tolprovided ;

public:

    explicit Rotate(double angle);
	explicit Rotate(double angle, double tolerance) ;
	
	void init(const Robot_inputs &in, Toplevel::Status_detail &status) ;
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
};
