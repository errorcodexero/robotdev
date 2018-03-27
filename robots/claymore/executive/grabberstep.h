#include "step.h"
#include <string>

//Move the grabber to a sepecified angle preset
struct GrabberStep: Step_impl_inner<GrabberStep>{

	bool anglegiven ;
	double angle ;
	std::string param ;
    GrabberController::Preset preset;
    bool init;

    explicit GrabberStep(GrabberController::Preset p);
    explicit GrabberStep(double a);
	explicit GrabberStep(const char *param_p, double a) ;

    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(GrabberStep const&)const;
};
