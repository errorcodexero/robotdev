#include "step.h"
#include <string>

//Start moving the lifter to a specified preset in the background
class LifterStep: public Step
{
private:
    LifterController::Preset preset;
	double height ;
	bool heightgiven ;
	bool background ;
	std::string param ;

public:
    explicit LifterStep(LifterController::Preset, bool back = false);
    explicit LifterStep(double, bool back = false);
    explicit LifterStep(const char *name_p, double def, bool back = false);

	virtual void init(const Robot_inputs &in, Toplevel::Status_detail &status) ;
	Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(LifterStep const&)const;
};

