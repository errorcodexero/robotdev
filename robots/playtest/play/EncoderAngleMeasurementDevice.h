#pragma once
#include "AngleMeasurementDevice.h"
#include "Encoder.h"
#include <memory>
namespace xerolib
{
	class EncoderAngleMeasurementDevice : public AngleMeasurementDevice
	{
	public:
		EncoderAngleMeasurementDevice(std::shared_ptr<frc::Encoder> left_p, std::shared_ptr<frc::Encoder> right_p);
		virtual ~EncoderAngleMeasurementDevice();

		virtual void reset()
		{
		}

		virtual double getAngle()
		{
			return 0.0;
		}

	private:
		std::shared_ptr<frc::Encoder> m_left_p;
		std::shared_ptr<frc::Encoder> m_right_p;
	};
}

