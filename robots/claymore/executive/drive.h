#include "step.h"

//Drive straight a specified distance
class Drive:public Step_impl_inner<Drive>{
protected:
    Inch mTargetDistance;
	Inch mTargetAngleOffset;
	Inch mReturnOffset;
	std::string mParamName ;
    bool mEndOnStall ;
	bool mCurve;
	Inch mCurveStart ;
	bool mReturnFromCollect ;
    bool mInited ;

public:
    explicit Drive(Inch dist, bool end_on_stall=false);
	explicit Drive(Inch dist, double curve_start, double angle_offset, bool end_on_stall=false);
    explicit Drive(const char *param_p, Inch dist, bool end_on_stall=false);
    explicit Drive(const std::string &param, Inch dist, bool end_on_stall=false);
	explicit Drive(bool dummy, Inch return_offset=0);

    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Drive const&)const;
};

