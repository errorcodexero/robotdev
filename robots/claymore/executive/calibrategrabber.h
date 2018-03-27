#include "step.h"

//Calibrate the grabber at the current angle
class Calibrate_grabber: public Step
{
private:
    bool mInited ;

public:
    explicit Calibrate_grabber();
	
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
};
