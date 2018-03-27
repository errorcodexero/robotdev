#include "step.h"

//Put the grabber into collecting mode until a cube is collected
class Collect: Step
{
private:
    double time;

public:

    explicit Collect(double);

	void init(const Robot_inputs &in, Toplevel::Status_detail &status);
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
};

//Drive forward and collect until a cube is collected
