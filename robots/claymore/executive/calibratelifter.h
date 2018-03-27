#include "step.h"

//Calibrate the lifter at the current height
struct Calibrate_lifter: Step_impl_inner<Calibrate_lifter>{
	bool mInited ;
    explicit Calibrate_lifter();
	
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Calibrate_lifter const&)const;
};

