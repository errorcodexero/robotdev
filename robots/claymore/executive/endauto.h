#include "step.h"


class EndAuto: public Step_impl_inner<EndAuto>{
public:
    EndAuto() ;
    
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(EndAuto const &)const;
} ;

