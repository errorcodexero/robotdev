#include "Joystick.h"

namespace frc
{
	Joystick::Joystick(int port) : GenericHID(port)
	{
	}

	Joystick::~Joystick()
	{
	}

	Joystick *Joystick::GetStickForPort(int port)
	{

	}

	void Joystick::SetXChannel(int channel)
	{

	}
	void Joystick::SetYChannel(int channel)
	{

	}
	void Joystick::SetZChannel(int channel)
	{

	}
	void Joystick::SetTwistChannel(int channel)
	{

	}
	void Joystick::Joystick::SetThrottleChannel(int channel)
	{

	}
	void SetAxisChannel(Joystick::AxisType type, int channel)
	{

	}
	int Joystick::GetXChannel() const
	{

	}
	int Joystick::GetYChannel() const
	{

	}
	int Joystick::GetZChannel() const
	{

	}
	int Joystick::GetTwistChannel() const
	{

	}
	int Joystick::GetThrottleChannel() const
	{

	}
	int Joystick::GetAxisChannel(AxisType axis) const
	{

	}
	double Joystick::GetX(JoystickHand hand) const
	{

	}
	double Joystick::GetY(JoystickHand hand) const
	{

	}
	double Joystick::GetX() const
	{

	}
	double Joystick::GetTwist() const
	{

	}
	double Joystick::GetThrottle() const
	{

	}
	double Joystick::GetAxis(AxisType type) const
	{

	}
	bool Joystick::GetTrigger() const
	{

	}
	bool Joystick::GetTriggerPressed()
	{

	}
	bool Joystick::GetTriggerReleased()
	{

	}
	bool Joystick::GetTop() const
	{

	}
	bool Joystick::GetTopPressed()
	{

	}
	bool Joystick::GetTopReleased()
	{

	}
	bool Joystick::GetButton(ButtonType type)
	{

	}
	double Joystick::GetMagnitude() const
	{

	}
	double Joystick::GetDirectionRadians() const
	{

	}
	double Joystick::GetDirectionDegrees() const
	{

	}
}
