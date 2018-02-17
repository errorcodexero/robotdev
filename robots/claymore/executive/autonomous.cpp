#include "autonomous.h"
#include "teleop.h" 
#include "chain.h"
#include "step.h"
#include "message_logger.h"

using namespace std;

//
// An auto mode program that drives straight to score on the
// switch on the same side as the robot
//
Executive same_switch
{
    Chain
    {
	vector<Step>
	{
	    Step
            {   
                Calibrate_lifter{}
            },
            Step
            {   
                Calibrate_grabber{}
            },
            Step
            {   
                Wait{1.0}
            },
            Step
            {   
                Start_lifter_in_background{LifterController::Preset::SWITCH, 0.0}
            },
            Step
            {   
                Drive{110.0} //ONLY FOR CORVALLIS - SHOULD BE 128
            },
            Step
            {   
                Wait_for_lifter{}
            },
            Step
            {   
                Eject{}
            }
	},
	Executive
	{
	    Teleop()
	}
    }
};

//
// An auto mode program that drives straight, turns left, drives straight, turns right
// drives straight and deposits a cube on the switch on the opposite side from the starting
// side of the robot
//
Executive opposite_switch
{
    Chain
    {
	vector<Step>
	{
	    Step
	    {
		Calibrate_grabber{}
	    },
	    Step
	    {
		Calibrate_lifter{}
	    },
	    Step
	    {
		Drive{52.0}
	    },
	    Step
	    {
		Wait{1.0}
	    },
	    Step
	    {
		Rotate{-90.0}
	    },
	    Step
	    {
		Wait{1.0}
	    },
	    Step
	    {
		Drive{100.0}
	    },
	    Step
	    {
		Wait{1.0}
	    },
	    Step
	    {
		Rotate{90.0}
	    },
	    Step
	    {
		Wait{1.0}
	    },
	    Step
	    {
                Start_lifter_in_background{LifterController::Preset::SWITCH, 0.0}
	    },
	    Step
	    {
		Drive{56}
	    },
	    Step
	    {
		Wait_for_lifter{}
	    },
	    Step
	    {
		Eject{}
	    }
	},
	Executive
	{
	    Teleop()
	}
    }
};
	

Executive make_test_step(auto a){
    return
	Executive
        {
	    Chain{
		Step
		{
		    a
		},
		Executive
		{
		    Teleop
		    {
		    }
		}
	    }
	};
}

Executive get_auto_mode(Next_mode_info info)
{
    messageLogger &logger = messageLogger::get() ;
    
    if(!info.autonomous)
    {
	logger.startMessage(messageLogger::messageType::info) ;
	logger << "get_auto_mode - not in autonomous, returning Teleop mode" ;
	logger.endMessage() ;
	return Executive{Teleop()};
    }

Executive auto_null{Teleop{}};

#if AUTOMODE_TEST == 0
    //
    // AUTOMODE_TEST == 0, null automode test program
    //
    logger.startMessage(messageLogger::messageType::info) ;
    logger << "get_auto_mode - AUTOMODE_TEST == 0, do nothing" ;
    logger.endMessage() ;
    
    const Executive auto_program
    {
	Teleop{}
    };
	
#elif AUTOMODE_TEST == 1
    //
    // AUTOMODE_TEST == 1, drive straight 12 inches
    //
    logger.startMessage(messageLogger::messageType::info) ;
    logger << "get_auto_mode - AUTOMODE_TEST == 1, drive 12 inches" ;
    logger.endMessage() ;
    
    Executive auto_program
    {
	Chain
	{
	    Step
	    {
		Drive{12.0}
	    },
	    Executive
	    {
		Teleop()
	    }
	}
    } ;

#elif AUTOMODE_TEST == 2
    //
    // AUTOMODE_TEST == 2, drive straight 10 feet
    //
    logger.startMessage(messageLogger::messageType::info) ;
    logger << "get_auto_mode - AUTOMODE_TEST == 2, drive 10 ft" ;
    logger.endMessage() ;
    
    Executive auto_program
    {
	Chain
	{
	    Step
	    {
		Drive{120.0}
	    },
	    Executive
	    {
		Teleop()
	    }
	}
    } ;

#elif AUTOMODE_TEST == 3
    //
    // AUTOMODE_TEST = 3, rotate 90 degrees
    //
    logger.startMessage(messageLogger::messageType::info) ;
    logger << "get_auto_mode - AUTOMODE_TEST == 3, rotate 90" ;
    logger.endMessage() ;
    
    Executive auto_program
    {
	Chain
	{
	    Step
	    {
		Rotate{90.0}
	    },
	    Executive
	    {
		Teleop()
	    }
	}
    };

#elif AUTOMODE_TEST == 4
    //
    // AUTOMODE_TEST = 4, rotate -90 degrees
    //
    logger.startMessage(messageLogger::messageType::info) ;
    logger << "get_auto_mode - AUTOMODE_TEST == 4, rotate -90" ;
    logger.endMessage() ;
    
    Executive auto_program
    {
	Chain
	{
	    Step
	    {
		Rotate{-90.0}
	    },
	    Executive
	    {
		Teleop()
	    }
	}
    };
#elif AUTOMODE_TEST == 5
    //
    // AUTOMODE_TEST = 5, rotate 90 degress, wait 2 seconds, rotate -90 degrees
    //
    logger.startMessage(messageLogger::messageType::info) ;
    logger << "get_auto_mode - AUTOMODE_TEST == 5, rotate 90, wait 2 secs, rotate -90" ;
    logger.endMessage() ;
    
    Executive auto_program
    {
	Chain
	{	
	    vector<Step>
	    {
		Step
		{
		    Rotate{-90.0}
		},
		Step
		{
		    Wait{2.0}
		},
		Step
		{
		    Rotate{90.0}
		},
		Step
		{
		    Wait{2.0}
		},
	    },
	    Executive
	    {
		Teleop()
	    }
	}
    };

#elif AUTOMODE_TEST == 6
   //
   // AUTOMODE_TEST = 6, start the lifter in the background, drive forward 60 inches, wait until the lifter has reached its goal
   //
   logger.startMessage(messageLogger::messageType::info);
   logger << "get_auto+mode - AUTOMODE_TEST == 6, start lifter, drive 60, wait for lifter" ;
   logger.endMessage();

   Executive auto_program
   {
	Chain
	{
	    vector<Step>
	    {
		Step
		{
		    Start_lifter_in_background{LifterController::Preset::SWITCH, info.in.now}
		},
		Step
		{
		    Drive{60.0}
		},
		Step
		{
		    Wait_for_lifter{}
		}
	    }
	},
	Executive
	{
	    Teleop()
	}
   };
    
#elif AUTOMODE_TEST == 7
    //
    // AUTOMODE_TEST = 7, calibrate the lifter, move the lifter up 43.5 inches
    //
    logger.startMessage(messageLogger::messageType::info);
    logger << "get_auto+mode - AUTOMODE_TEST == 7, calibrate the lifter, move the lifter up 43.5 inches" ;
    logger.endMessage();

    Executive auto_program
    {
 	Chain
 	{
 	    vector<Step>
 	    {
 		Step
 		{
		    Calibrate_lifter{}
		},
		Step
		{
		    Lifter_to_height{24, info.in.now}
		},
		Step
		{
		    Wait{2.0}
		},
		Step
		{
		    Lifter_to_height{43.5, info.in.now}
		},
		Step
		{
		    Wait{2.0}
		},
		Step
		{
		    Lifter_to_height{24, info.in.now}
		}
	    },
	    Executive
	    {
	        Teleop()
	    }
	}
    };

#elif AUTOMODE_TEST == 8
    //
    // AUTOMODE_TEST = 8, calibrate the grabber, move it to the open position
    //
    logger.startMessage(messageLogger::messageType::info);
    logger << "get_auto_mode - AUTOMODE_TEST == 8, calibrate the grabber, move it to the open position" ;
    logger.endMessage();

    Executive auto_program
    {
	Chain
	{
	    vector<Step>
	    {
		Step
		{
		    Calibrate_lifter{}
		},
		Step
		{
		    Calibrate_grabber{}
		}/*,
		Step
		{
		    Wait{1.0}
		},
		Step
		{
		    Start_lifter_in_background{LifterController::Preset::SWITCH, info.in.now}
		},
		Step
		{
		    Drive{128.0}
		},
		Step
		{
		    Wait_for_lifter{}
		},
		Step
		{
		    Eject{}
		}*/
	    },
	    Executive
	    {
		Teleop()
	    }
	}
    };

#elif AUTOMODE_TEST == 20
    //
    // AUTOMODE_TEST = 20, run the auto program for switch same side
    //
    logger.startMessage(messageLogger::messageType::info) ;
    logger << "get_auto_mode - AUTOMODE_TEST == 20, switch same side" ;
    logger.endMessage() ;
    
    Executive auto_program = same_switch ;
	
#elif AUTOMODE_TEST == 21
    //
    // AUTOMODE_TEST = 21, run the auto program for switch same side
    //
    logger.startMessage(messageLogger::messageType::info) ;
    logger << "get_auto_mode - AUTOMODE_TEST == 21, switch opposite side" ;
    logger.endMessage() ;
    
    Executive auto_program = opposite_switch ;
	
#elif AUTOMODE_TEST == 22
     //
     // AUTOMODE_TEST = 22, decide between same side and opposite side for switch
     //
    logger.startMessage(messageLogger::messageType::info) ;
    logger << "get_auto_mode - AUTOMODE_TEST == 22, decide switch side" ;
    logger.endMessage() ;
    
    Executive auto_program = info.in.ds_info.near_switch_left ? opposite_switch : same_switch ;

#else
    //
    // If AUTOMODE_TEST was not defined, we revert to the default behavior which is
    // to run the with the panel
    //
    
    logger.startMessage(messageLogger::messageType::info) ;
    logger << "get_auto_mode - competition mode, selecting auto mode based on switch" ;
    logger.endMessage() ;

    return info.in.ds_info.near_switch_left ? opposite_switch : same_switch ;
	
    if(!info.panel.in_use) {
	//
	// No panel was detected.  May be a problem with the driver station, log this fact
	// and do nothing during autonomous
	//
	logger.startMessage(messageLogger::messageType::error) ;
	logger << "get_auto_mode - no panel detected, defaulting to null auto program" ;
	logger.endMessage() ;
	
    	return info.in.ds_info.near_switch_left ? opposite_switch : same_switch ;
	//return auto_null;
    }

    logger.startMessage(messageLogger::messageType::error) ;
    logger << "get_auto_mode - panel value is " << info.panel.auto_select ;
    logger.endMessage() ;
    
    Executive auto_program = info.in.ds_info.near_switch_left ? opposite_switch : same_switch ;

    switch(info.panel.auto_select){
    case 0: 
	return auto_null;
    case 1:
    case 2: 
    case 3:
    case 4: 
    case 5: 
    case 6:
    case 7: 
    case 8: 
    case 9: 
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    default:
	return auto_null;
    }
#endif
	
    return auto_program ;
}

Executive Autonomous::next_mode(Next_mode_info info)
{
    static const Time DELAY = 0.0;//seconds, TODO: base it off of the dial on the OI? 
    if(!info.autonomous) return Executive{Teleop()};
    if(info.since_switch > DELAY) return get_auto_mode(info);
    return Executive{Autonomous()};
}

Toplevel::Goal Autonomous::run(Run_info)
{	
    return {} ;
}

bool Autonomous::operator<(Autonomous const&)const
{
	return 0;
}

bool Autonomous::operator==(Autonomous const&)const
{
	return 1;
}

void Autonomous::display(ostream& o)const
{
    o<<"Autonomous()";
}


#ifdef AUTONOMOUS_TEST
#include "test.h"
int main()
{
    Autonomous a;
    test_executive(a);
}
#endif 
