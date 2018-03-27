#include "step.h"

//Rotate the robot by a specified angle
struct Rotate_back: Step_impl_inner<Rotate_back>{
    bool init;
	double mOffset ;
	double mTolerance ;
	bool mTolSpecified ;

    explicit Rotate_back() ;
	explicit Rotate_back(double offset) ;
	explicit Rotate_back(double offset, double tolerance) ;
	
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Rotate_back const&)const;
};


