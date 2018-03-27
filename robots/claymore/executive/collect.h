#include "step.h"

//Put the grabber into collecting mode until a cube is collected
struct Collect: Step_impl_inner<Collect>{
    double time;
    bool init;

    explicit Collect(double);

    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Collect const&)const;
};

//Drive forward and collect until a cube is collected
