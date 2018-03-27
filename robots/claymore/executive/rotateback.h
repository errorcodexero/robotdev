#include "step.h"

//Rotate the robot by a specified angle
class Rotate_back: Step
{
private:
	double mOffset ;
	double mTolerance ;
	bool mTolSpecified ;

public:
    explicit Rotate_back() ;
	explicit Rotate_back(double offset) ;
	explicit Rotate_back(double offset, double tolerance) ;
	
	virtual void init(const Robot_inputs &in, Toplevel::Status_detail &status) ;
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Rotate_back const&)const;
};


