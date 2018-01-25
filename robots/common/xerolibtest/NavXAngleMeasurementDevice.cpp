#include "NavXAngleMeasurementDevice.h"
#include <stdexcept>

namespace xerolib
{
    NavXAngleMeasurementDevice::NavXAngleMeasurementDevice()
    {
	m_navx_p = new AHRS(frc::SerialPort::Port::kUSB);

	auto start = std::chrono::high_resolution_clock::now();
	while (m_navx_p->IsCalibrating())
	{
	    auto duration = std::chrono::high_resolution_clock::now() - start;
	    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

	    if (ms.count() > CalibrationTimeout)
	    {
		delete m_navx_p;
		m_navx_p = nullptr;

		std::string msg("timeout during navx calibration");
		std::runtime_error ex(msg);
		throw ex;
	    }
	}
    }

    NavXAngleMeasurementDevice::~NavXAngleMeasurementDevice()
    {
	if (m_navx_p != nullptr)
	    delete m_navx_p;
    }
}
