#include "GenericHID.h"

namespace frc
{
	GenericHID::GenericHID(int port)
	{
	}

	GenericHID::~GenericHID()
	{
	}

	bool GenericHID::GetRawButton(int button) const
	{
	}

	bool GenericHID::GetRawButtonPressed(int button) const
	{
	}

	bool GenericHID::GetRawButtonReleased(int button) const
	{
	}

	double GenericHID::GetRawAxis(int axis) const
	{
	}

	int GenericHID::GetPOV(int pov) const
	{
	}

	int GenericHID::GetAxisCount() const
	{
	}

	int GenericHID::GetPOVCount() const
	{
	}

	int GenericHID::GetButtonCount() const
	{
	}

	GenericHID::HIDType GenericHID::GetType() const
	{
	}

	std::string GenericHID::GetName() const
	{
	}

	int GenericHID::GetAxisType() const
	{
	}

	int GenericHID::GetPort() const
	{
	}

	int GenericHID::SetOutput(int outputNumber, bool value) const
	{
	}

	int GenericHID::SetOutputs(int value) const
	{
	}

	void GenericHID::SetRumble(RumbleType type, double value)
	{
	}
}
