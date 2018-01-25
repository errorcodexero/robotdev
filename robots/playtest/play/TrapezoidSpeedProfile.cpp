#include "TrapezoidSpeedProfile.h"
#include <cmath>
#include <iostream>

namespace xerolib
{
	TrapezoidSpeedProfile::TrapezoidSpeedProfile()
	{
		m_type = ProfileType::None;
	}

	TrapezoidSpeedProfile::~TrapezoidSpeedProfile()
	{
	}

	double TrapezoidSpeedProfile::getTargetSpeed(double time) const
	{
		double ret = 0.0;

		if (time < 0)
		{
			ret = m_vs;
		}
		else if (time < m_ta)
		{
			//
			// In the acceleration phase
			//
			ret = m_vs + time * m_aa;
		}
		else if (time >= m_ta && time < m_ta + m_tc)
		{
			//
			// In the cruising phase
			//
			ret = m_vt;
		}
		else if (time >= m_ta + m_tc && time < m_ta + m_tc + m_td)
		{
			//
			// In the deceleration phase
			//
			ret = m_vt + (time - m_ta - m_tc) * m_ad;
		}
		else
		{
			ret = m_ve;
		}

		return ret;
	}

	double TrapezoidSpeedProfile::getTargetDistance(double time) const
	{
		double ret = 0.0;

		if (time < 0)
		{
			ret = 0.0;
		}
		else if (time < m_ta)
		{
			//
			// In the acceleration phase
			//
			ret = m_vs * time + 0.5 * m_aa * time * time;
		}
		else if (time >= m_ta && time < m_ta + m_tc)
		{
			//
			// In the cruising phase
			//
			ret = m_da + (time - m_ta) * m_vt;
		}
		else if (time >= m_ta + m_tc && time < m_ta + m_tc + m_td)
		{
			//
			// In the deceleration phase
			//
			double dt = (time - m_ta - m_tc);
			ret = m_da + m_dc + m_vt * dt + 0.5 * dt * dt * m_ad;
		}
		else
		{
			ret = m_da + m_dc + m_dd;
		}

		return ret;
	}

	void TrapezoidSpeedProfile::calcProfile(double vs, double ve, double dist)
	{
		m_type = ProfileType::Trapezoid;

		if (vs > m_vc)
			vs = m_vc;

		m_dist = dist;
		m_vs = vs;
		m_ve = ve;

		m_vt = m_vc;
		m_ta = (m_vc - m_vs) / m_aa;
		m_da = (m_vs * m_ta + 0.5 * m_aa * m_ta * m_ta);

		m_td = (m_ve - m_vc) / m_ad;
		m_dd = m_vc * m_td + 0.5 * m_ad * m_td * m_td;

		m_tc = (m_dist - m_da - m_dd) / m_vc;
		if (m_tc < 0)
		{
			double num = (2 * m_dist * m_aa * m_ad + m_ad * m_vs * m_vs - m_aa * m_ve * m_ve) / (m_ad - m_aa);
			m_vt = std::sqrt(num);

			if (m_vt < m_vs)
			{
				//
				// This is a straight line deceleration to zero
				//
				m_ta = 0;
				m_tc = 0;
				m_td = (m_ve - m_vs) / m_ad;
				m_da = 0;
				m_dc = 0;
				m_dd = m_dist;
				m_type = ProfileType::Down;
				m_vt = m_vs;

				std::cout << "Straight";
				std::cout << ", vt=" << m_vt;
				std::cout << ", vs=" << m_vs;
				std::cout << ", td=" << m_td;
				std::cout << ", dd=" << m_dd;
				std::cout << std::endl ;
			}
			else
			{
				//
				// This is a V shape, up to Vt and down to Ve
				//
				m_ta = (m_vt - m_vs) / m_aa;
				m_td = (m_ve - m_vt) / m_ad;
				m_tc = 0;
				m_dc = 0;

				m_da = (m_vs * m_ta + 0.5 * m_aa * m_ta * m_ta);
				m_dd = m_vt * m_td + 0.5 * m_ad * m_td * m_td;
				m_type = ProfileType::Triangle;
			}
		}
		else
		{
			m_dc = m_tc * m_vc;
		}
	}

	double TrapezoidSpeedProfile::calcDistance() const
	{
		double dist = 0.0;

		dist += m_vs * m_ta + 0.5 * m_aa * m_ta * m_ta;
		dist += m_vt * m_tc;
		dist += m_vt * m_td + 0.5 * m_ad * m_td * m_td;
		return dist;
	}

	std::string TrapezoidSpeedProfile::toString() const
	{
		std::string ret;

		if (m_type == ProfileType::None)
			ret = "Type: None\n";
		else
		{
			if (m_type == ProfileType::Triangle)
				ret = "Type: Triangle";
			else
				ret = "Type: Trapezoid";
			ret += "\n";

			ret += "  Ta ";
			ret += std::to_string(m_ta);
			ret += "\n";

			ret += "  Tc ";
			ret += std::to_string(m_tc);
			ret += "\n";

			ret += "  Td ";
			ret += std::to_string(m_td);
			ret += "\n";

			ret += "  Vt ";
			ret += std::to_string(m_vt);
			ret += "\n";

			ret += "  Da ";
			ret += std::to_string(m_da);
			ret += "\n";

			ret += "  Dc ";
			ret += std::to_string(m_dc);
			ret += "\n";

			ret += "  Dd ";
			ret += std::to_string(m_dd);
			ret += "\n";
		}

		return ret;
	}
}
