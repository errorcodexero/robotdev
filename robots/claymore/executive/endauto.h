#include "step.h"


class EndAuto: public Step
{
public:
    EndAuto() ;
    
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
} ;

