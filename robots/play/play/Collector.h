#include <SubsystemBase.h>
#include <DigitalInput.h>
#include <memory>

class Intake ;
class Grabber ;

class Collector : public xerolib:SubsystemBase
{
public:
	Collector(xerolib::XeroRobotBase &robot,
			  std::shared_ptr<Intake> intake_p,
			  std::shared_ptr<Grabber> grabber_p,
			  std::shared_ptr<frc::DigitalInput> detect_p) ;

	virtual ~Collector() ;

	virtual void getInputs() ;
	virtual void setOutputs() ;

	bool isDone() const ;

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

	void collect(double angle, dobule speed)
	{
	}

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

private:
	State m_state ;
	
	std::shared_ptr<Intake> m_intake_p ;
	std::shared_ptr<Grabber> m_grabber_p ;
	std::shared_ptr<frc::DigitalInput> m_cube_detect_p ;

	double m_default_collect_speed ;
	double m_default_drop_speed ;
	double m_default_drop_time ;
	double m_default_eject_speed ;
	double m_default_eject_time ;
}
