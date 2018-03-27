#include "step.h"

class StartAuto: public Step_impl_inner<StartAuto>{
public:
	std::string mProgramName ;
	bool mInited ;
	
    StartAuto(const char *name_p) ;
    
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(StartAuto const &)const;
} ;
