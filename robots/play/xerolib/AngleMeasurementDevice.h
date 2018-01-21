#pragma once
class AngleMeasurementDevice
{
public:
	AngleMeasurementDevice()
	{
	}

	virtual ~AngleMeasurementDevice()
	{
	}

	virtual void reset() = 0;
	virtual double getAngle() = 0;
};

