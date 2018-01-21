#include "EncoderAngleMeasurementDevice.h"

namespace xerolib
{
	EncoderAngleMeasurementDevice::EncoderAngleMeasurementDevice(std::shared_ptr<frc::Encoder> left_p, std::shared_ptr<frc::Encoder> right_p)
	{
		m_left_p = left_p;
		m_right_p = right_p;
	}

	EncoderAngleMeasurementDevice::~EncoderAngleMeasurementDevice()
	{
	}
}