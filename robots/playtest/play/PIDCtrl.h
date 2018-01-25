#pragma once

namespace xerolib
{
	class PIDCtrl
	{
	public:
		PIDCtrl();
		~PIDCtrl();

		void setConstants(double p, double i, double d)
		{
			m_kp = p;
			m_ki = i;
			m_kd = d;
			m_kf = 0.0;
			m_sumi = 0.0;
		}

		void setConstants(double p, double i, double d, double f)
		{
			m_kp = p;
			m_ki = i;
			m_kd = d;
			m_kf = f;
			m_sumi = 0.0;
		}

		void setIGuard(double v)
		{
			m_i_guard = v;
		}

		double calcOutput(double deltat, double target, double actual);

		double getLastError() const
		{
			return m_lasterror;
		}

		double getLastDerError() const
		{
			return m_last_derror;
		}

		double getLastSumI() const
		{
			return m_sumi;
		}

	private:
		double m_kp;
		double m_ki;
		double m_kd;
		double m_kf;

		double m_i_guard;

		double m_sumi;
		double m_lasterror;

		double m_last_derror;
	};
}