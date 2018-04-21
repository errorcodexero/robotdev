#include "SubsystemBase.h"
#include <PIDCtrl.h>
#include <SpeedController.h>
#include <Encoder.h>
#include <DigitalInput.h>
#include <Solenoid.h>
#include <list>
#include <memory>
#include <map>

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
		   std::shared_ptr<frc::DigitalInput> upper_limit_p,
		   std::shared_ptr<frc::Solenoid> brake_p,
		   std::shared_ptr<frc::Solenoid> shifter_p,
		   bool quick = true,
		   size_t nSamples = 3) ;

	virtual ~Lifter() ;

	virtual void getInputs() ;
	virtual void setOutputs() ;

	double getHeight() const
	{
		if (!m_calibrated)
			return std::nan("") ;

		return m_actual_height ;
	}

	void gotoHeight(Preset pre)
	{
		gotoHeight(presetToHeight(pre)) ;
	}
	
	void gotoHeight(double height) ;

	void calibrate()
	{
		m_state = State::CALIBRATING ;
	}

	void calibrate(bool quick)
	{
		m_quick_calibrate = quick ;
		m_state = State::CALIBRATING ;
	}
	
	void setClimbBaseline()
	{
		m_climb_baseline = m_encoder_p->Get() ;
		m_climb_baseline_set = true ;
	}

	void up()
	{
		m_state = State::UP ;
	}

	void down()
	{
		m_state = State::DOWN ;
	}

private:
	static constexpr double INCHES_PER_TICK_HIGH_GEAR = 0.08327 ;
	static constexpr double COLLECTOR_BOTTOM_OFFSET = 11.375 ;
	
	enum class State
	{
		//
		// Uninitialized
		//
		UNINITIALIZED,
			
		//
		// Actively calibrating the lifter
		//
		CALIBRATING,

		//
		// Seeking a target height
		//
		SEEKING,

		//
		// Holding at a fixed target height
		//
		HOLDING,

		//
		// Sustain the current height, used after a climb
	    //
		SUSTAIN,

		//
		// Move the lifter up (manual)
		//
		UP,
			
		//
		// Move the lifter down (manual)
		//
		DOWN,
	} ;

	enum class ShifterState
>	{
		UNKNOWN,
		HIGH,
		LOW
	} ;

	enum class BrakeState
	{
		UNKNOWN,
		ON,
		OFF
	} ;
	
	double presetToHeight(Preset pre)
	{
		auto it = m_height_map.find(pre) ;
		if (it == m_height_map.end())
			return 12.0 ;

		return it->second ;
	}

	void setMotors(double v) ;
	void doCalibrate() ;
	void doSeek() ;
	void doHold() ;
	void doSustain() ;
	void doUp() ;
	void doDown() ;

	bool isStationary() ;

	void brakeOn()
	{
		if (m_brake != BrakeState::ON)
			m_brake_p->Set(false) ;

		m_brake = BrakeState::ON ;
	}

	void brakeOff()
	{
		if (m_brake != BrakeState::OFF)
			m_brake_p->Set(true) ;

		m_brake = BrakeState::OFF ;
	}

	void highGear()
	{
		if (m_shifter != ShifterState::HIGH)
			m_shifter_p->Set(false) ;

		m_shifter = ShifterState::HIGH ;
	}

	void lowGear()
	{
		if (m_shifter != ShifterState::LOW)
			m_shifter_p->Set(true) ;
		
		m_shifter = ShifterState::LOW ;
		m_calibrated = false ;
	}

private:
	//
	// The motors driving the lifter
	//
	std::list<std::shared_ptr<frc::SpeedController>> m_motors ;

	//
	// The encoder giving lifter position
	//
	std::shared_ptr<frc::Encoder> m_encoder_p ;

	//
	// The lower limit switch
	//
	std::shared_ptr<frc::DigitalInput> m_lower_limit_p ;

	//
	// The upper limit switch
	//
	std::shared_ptr<frc::DigitalInput> m_upper_limit_p ;

	//
	// The solenoid for the brake
	//
	std::shared_ptr<frc::Solenoid> m_brake_p ;

	//
	// The solenoid for the shifter
	//
	std::shared_ptr<frc::Solenoid> m_shifter_p ;

	//
	// The PID controller for the list
	//
	xerolib::PIDCtrl m_pid ;

	//
	// The map from height presets to height values
	//
	std::map<Preset, double> m_height_map ;

	//
	// If true, the lifter is calibrated
	//
	bool m_calibrated ;

	//
	// If true, do a quick calibrate
	//
	bool m_quick_calibrate ;

	//
	// If true, the climb baseline has been set
	//
	bool m_climb_baseline_set ;

	//
	// The climb baseline in ticks, used in the SUSTAIN state
	//
	int32_t m_climb_baseline ;

	//
	// The current state of the lifter
	//
	State m_state ;

	//
	// the tick count at the lifters lowest position
	//
	int32_t m_ticks_base ;

	//
	// The current height of the lifter
	//
	double m_actual_height ;

	//
	// The target height of the lifter
	//
	double m_target_height ;

	//
	// Current state of the shifter
	//
	ShifterState m_shifter ;

	//
	// Current state of the brake
	//
	BrakeState m_brake ;

	//
	// Samples from the encoder, used for calibration
	//
	std::vector<int32_t> m_samples ;

	//
	// The number of samples taken so far
	//
	size_t m_samples_taken ;

	//
	// The threshold for considering the lifter stationary
	//
	int32_t m_stationary_threshold ;
} ;
