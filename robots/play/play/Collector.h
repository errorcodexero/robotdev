#include <CountdownTimer.h>
#include <SubsystemBase.h>
#include <DigitalInput.h>
#include <memory>

class Intake ;
class Grabber ;

class Collector : public xerolib::SubsystemBase
{
public:
	Collector(xerolib::XeroRobotBase &robot,
			  std::shared_ptr<Intake> intake_p,
			  std::shared_ptr<Grabber> grabber_p,
			  std::shared_ptr<frc::DigitalInput> detect_p) ;

	virtual ~Collector() ;

	virtual void getInputs() ;
	virtual void setOutputs() ;

	void calibrate() ;

	void eject()
	{
		eject(m_default_eject_speed, m_default_eject_time) ;
	}
	
	void eject(double speed, double etime) ;
	
	void drop()
	{
		drop(m_default_drop_speed, m_default_drop_time) ;
	}
	
	void drop(double speed, double etime) ;
	
	void openCollect()
	{
		openCollect(m_default_collect_speed) ;
	}
	void openCollect(double speed) ;
	
	void closeCollect()
	{
		closeCollect(m_default_collect_speed) ;
	}
	
	void closeCollect(double speed) ;

	void collect(double angle, double speed) ;

	bool isDone() const
	{
		return m_state == State::IDLE ;
	}

private:
	enum class State
	{
		IDLE,
		EJECTING,
		DROPPING,
		COLLECTING,
		STOWED,
	 } ;

	enum class CubeStatus
	{
		NoCube,
		MaybeHasCube,
		GraspCube,
		HasCube,
		MaybeLostCube,
	} ;

	void pulseIntakeInit(double now) ;
	void pulseIntake(double now) ;

private:
	State m_state ;
	CubeStatus m_cube_state ;
	
	std::shared_ptr<Intake> m_intake_p ;
	std::shared_ptr<Grabber> m_grabber_p ;
	std::shared_ptr<frc::DigitalInput> m_cube_detect_p ;

	double m_default_collect_speed ;
	double m_default_drop_speed ;
	double m_default_drop_time ;
	double m_default_eject_speed ;
	double m_default_eject_time ;
	double m_maybe_has_cube_time ;
	double m_maybe_lost_cube_time ;
	double m_has_cube_intake_run_interval ;
	double m_has_cube_intake_run_duration ;
	double m_pulse_intake_speed ;

	xerolib::CountdownTimer m_cube_timer ;
	xerolib::CountdownTimer m_pulse_intake_timer ;
} ;
