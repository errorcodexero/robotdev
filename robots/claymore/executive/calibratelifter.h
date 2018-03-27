#include "step.h"

//Calibrate the lifter at the current height
class Calibrate_lifter: public Step
{
private:
	bool mInited ;

public:
    explicit Calibrate_lifter();
	
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
};

