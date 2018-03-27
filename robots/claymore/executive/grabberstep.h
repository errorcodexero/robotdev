#include "step.h"
#include <string>

//Move the grabber to a sepecified angle preset
class GrabberStep: Step
{
private:
	bool anglegiven ;
	double angle ;
	std::string param ;
    GrabberController::Preset preset;

public:
    explicit GrabberStep(GrabberController::Preset p);
    explicit GrabberStep(double a);
	explicit GrabberStep(const char *param_p, double a) ;

	void init(const Robot_inputs &in, Toplevel::Status_detail &status) ;


    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
};
