#include "SubsystemBase.h"

class Lifter : public xerolib::SubsystemBase
{
public:
	enum class Preset
	{
		FLOOR,
		EXCHANGE,
		SWITCH,
		SCALELOW,
		SCALEHIGH,
		CLIMB
	} ;

public:
	Lifter(xerolib::XeroRobotBase &robot,
		   std::shared_ptr<frc::SpeedController> motor1_p,
		   std::shared_ptr<frc::SpeedController> motor2_p,
		   std::shared_ptr<frc::Encoder> encoder_p,
		   std::shared_ptr<frc::DigitalInput> lower_limit_p,
		   std::shared_ptr<frc::DigitalInput> upper_limit_p) ;

	virtual ~Lifter() ;

	virtual void getInputs() ;
	virtual void getOutputs() ;
}

		   
