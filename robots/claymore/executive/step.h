#ifndef STEP_H
#define STEP_H

#include "executive.h"
#include "motion_profile.h"
#include "settable_constant.h"
#include "robot_constants.h"
#include <string>

using Inch=double;

struct Step_impl;

class Step
{
private:
	double mStartTime ;
	std::string mStepName ;
	
public:
    enum class Status
	{
		UNFINISHED,
		FINISHED_SUCCESS,
		FINISHED_FAILURE
	};

public:
	Step(const char *name_p)
	{
		mStepName = name_p ;
	}
	
	virtual void init(const Robot_inputs &in, Toplevel::Status_detail &status)
	{
	}
			
    virtual Toplevel::Goal run(Run_info,Toplevel::Goal) = 0 ;
    virtual Toplevel::Goal run(Run_info) = 0 ;
    virtual Status done(Next_mode_info) = 0 ;

	virtual void startStep(double time)
	{
		mStartTime = time ;
	}
	
	virtual void finishStep(double time)
	{
	}
};

std::ostream& operator<<(std::ostream&,Step const&);

//Run two steps simultaneously
class Combo: public Step
{
    Step &step_a;
    Step &step_b;
	
public:
    explicit Combo(Step &, Step &);
	explicit Combo(const char *name_p, Step &, Step &);

	virtual void init(const Robot_inputs &in, Toplevel::Status_detail &status);
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);

	virtual void startStep(double time)
	{
		step_a.startStep(time) ;
		step_b.startStep(time) ;
		Step::startStep(time) ;
	}
	
	virtual void finishStep(double time)
	{
		step_a.finishStep(time) ;
		step_b.finishStep(time) ;
		Step::finishStep(time) ;
	}
	
};


#endif
