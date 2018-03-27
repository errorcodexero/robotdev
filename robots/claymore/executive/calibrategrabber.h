#include "step.h"

//Calibrate the grabber at the current angle
struct Calibrate_grabber: Step_impl_inner<Calibrate_grabber>{
    bool mInited ;
    
    explicit Calibrate_grabber();
	
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Calibrate_grabber const&)const;
};
