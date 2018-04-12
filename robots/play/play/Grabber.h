#include "SubsystemBase.h"
#include <PIDCtrl.h>
#include <SpeedController.h>
#include <Encoder.h>
#include <memory>
#include <vector>
#include <cstdint>

class Grabber : public xerolib::SubsystemBase
{
public:
	enum class Preset
	{
		CLOSED,
		OPEN,
		STOWED
	} ;
	
public:
	Grabber(xerolib::XeroRobotBase &robot,
			std::shared_ptr<frc::SpeedController> motor_p,
			std::shared_ptr<frc::Encoder> encoder_p,
			size_t samples) ;
	virtual ~Grabber() ;


	virtual void getInputs() ;
	virtual void setOutputs() ;

	bool isFolded() const
	{
		return isCalibrated() && getCurrentAngle() < m_folded_angle ;
	}

	bool isCalibrated() const
	{
		return m_calibrated ;
	}

	bool isIdle() const
	{
		return m_state == State::Idle ;
	}

	bool isHolding() const
	{
		return m_state == State::Hold ;
	}

	void hold()
	{
		m_state = State::Hold ;
	}

	void grasp()
	{
		m_state = State::Grasp ;
	}

	void idle()
	{
		m_state = State::Idle ;
	}

	void calibrate()
	{
		m_state = State::Calibrate ;
	}
	
	void gotoAngle(double angle)
	{
		m_target = angle ;
		m_state = State::Angle ;
	}
	
	void gotoAngle(Grabber::Preset preset)
	{
		m_target = presetToAngle(preset) ;
		m_state = State::Angle ;
	}

	double getCurrentAngle() const
	{
		return m_angle ;
	}

private:
	static constexpr double kDelta = 1e-6 ;
	static constexpr double kDegreesPerTick = 90.0 / 144.0 ;
	
	//
	// The state of the grabber
	//
	enum class State
	{
		Idle,
		Calibrate,
		Hold,
		Grasp,
		Angle,
	} ;
	
	double presetToAngle(Grabber::Preset preset)
	{
		double ret = m_open_angle ;

		switch(preset)
		{
		case Preset::OPEN:
			ret = m_open_angle ;
			break ;
		case Preset::CLOSED:
			ret = m_closed_angle ;
			break ;
		case Preset::STOWED:
			ret = m_stowed_angle ;
			break ;
		}

		return ret ;
	}

	void setMotorVoltage(double v) ;
	void doAngle() ;
	void doGrasp() ;
	void doCalibration() ;
	bool isStationary() ;

private:

	//
	// The state of the grabber
	//
	State m_state ;

	//
	// If true, the grabber is calibrated
	//
	bool m_calibrated ;

	//
	// The target angle
	//
	double m_target ;
	
	//
	// The motor for the grabber
	//
	std::shared_ptr<frc::SpeedController> m_motor_p ;

	//
	// The encoder for the motor
	//
	std::shared_ptr<frc::Encoder> m_encoder_p ;

	//
	// The holding voltage
	//
	double m_holding_voltage ;

	//
	// The grasping voltage
	//
	double m_grasping_voltage ;

	//
	// The angles for the various presets
	//
	double m_open_angle ;
	double m_closed_angle ;
	double m_stowed_angle ;
	double m_folded_angle ;

	//
	// The last voltage applied to the motor
	//
	double m_last_voltage ;

	//
	// The last N samples
	//
	std::vector<int32_t> m_samples ;

	//
	// The number of samples in the array above
	//
	size_t m_samples_taken ;

	//
	// The encoder value that is the base offset for zero degrees
	//
	int32_t m_encoder_base ;

	//
	// The calibration threshold for the encoder samples
	//
	int32_t m_stationary_threshold ;

	//
	// The latest angle for the grabber
	//
	double m_angle ;

	//
	// The PID controller for the grabber
	//
	xerolib::PIDCtrl m_pid ;

	//
	// The last time through the robot loop
	//
	double m_last_time ;
} ;

 
