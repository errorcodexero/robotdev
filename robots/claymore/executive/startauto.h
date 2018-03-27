#include "step.h"

class StartAuto: public Step
{
public:
	std::string mProgramName ;
	bool mInited ;
	
public:
    StartAuto(const char *name_p) ;
    
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
} ;
