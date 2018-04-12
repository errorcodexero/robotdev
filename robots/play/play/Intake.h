#include <SubsystemBase.h>
#include <SpeedController.h>
#include <memory>

class Intake : public xerolib::SubsystemBase
{
public:
	Intake(xerolib::XeroRobotBase &base,
		   std::shared_ptr<frc::SpeedController> left_p,
		   std::shared_ptr<frc::SpeedController> right_p) ;
	virtual ~Intake() ;


	virtual void getInputs() ;
	virtual void setOutputs() ;

	virtual void run(double speed) ;
	virtual void run(double speed, double duration) ;

	bool isRunning() const ;

private:
	static constexpr double kDelta = 1e-6 ;

	void writeMotors(double value) ;

private:
	//
	// The motor on the left side of the robot
	//
	std::shared_ptr<frc::SpeedController> m_left_p ;

	//
	// The motor on the right side of the robot
	//
	std::shared_ptr<frc::SpeedController> m_right_p ;

	//
	// The percent of the PWM to be applied to the intake motors
	//
	double m_speed ;

	//
	// The length of time to run the intake
	//
	double m_duration ;

	//
	// The time when the intakes were started
	//
	double m_started ;

	//
	// The last value written to the PWM
	//
	double m_last_value ;
};

