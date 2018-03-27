#include "step.h"

//Eject a cube
struct Eject: Step_impl_inner<Eject>
{
	enum class EjectState
	{
		//
		// We are ready to start an eject operation
		//
		Start,

		//
		// We are waiting on a timer to expire to be done
		//
		WaitingOnTime,

		//
		// Waiting for the cube sensor to turn off (or the timer)
		//
		WaitingCubeSensorOff,

		//
		// Eject is done
		//
		Done
	} ;
	
	EjectState mState ;
    Countdown_timer eject_timer;
	double mStart ;
	bool mPowerApplied ;
	double mPower ;

    explicit Eject();
	explicit Eject(double power) ;
	
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Eject const&)const;
};

