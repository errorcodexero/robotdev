#include "PIDCtrl.h"
#include <limits>

namespace xerolib
{
	PIDCtrl::PIDCtrl()
	{
		m_i_guard = std::numeric_limits<double>::max() / 2.0;
		m_sumi = 0.0;
		m_lasterror = 0.0;
	}

	PIDCtrl::~PIDCtrl()
	{
	}

	double PIDCtrl::calcOutput(double deltat, double target, double actual)
	{
		double error = target - actual;
		double derror = (error - m_lasterror) / deltat;

		if (derror > 1e26)
			derror = 1e26;

		m_sumi += error * deltat;
		if (m_sumi < -m_i_guard)
			m_sumi = -m_i_guard;
		else if (m_sumi > m_i_guard)
			m_sumi = m_i_guard;

		m_lasterror = error;
		m_last_derror = derror;
		return m_kp * error + m_ki * m_sumi + m_kd * derror + m_kf * target;
	}
}
