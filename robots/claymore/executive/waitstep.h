#include "step.h"

//Wait for a specified amount of time
class Wait: public Step_impl_inner<Wait>{
    Countdown_timer wait_timer;
public:
    explicit Wait(Time);

    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Wait const&)const;
};

