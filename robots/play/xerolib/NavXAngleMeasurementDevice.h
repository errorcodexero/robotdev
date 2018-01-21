#pragma once

#include "AngleMeasurementDevice.h"
#include <AHRS.h>

namespace xerolib
{
	class NavXAngleMeasurementDevice : public AngleMeasurementDevice
	{
	public:
		NavXAngleMeasurementDevice();
		virtual ~NavXAngleMeasurementDevice();

		virtual void reset()
		{
			m_navx_p->ZeroYaw();
		}

		virtual double getAngle()
		{
			return m_navx_p->GetYaw();
		}

	private:
		static const int CalibrationTimeout = 3000;

	private:
		AHRS * m_navx_p;
	};
}

