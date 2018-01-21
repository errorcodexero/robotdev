#pragma once

#include <string>

namespace xerolib
{
	class TrapezoidSpeedProfile
	{
	public:
		enum class ProfileType
		{
			None,
			Down,
			Triangle,
			Trapezoid
		};

	public:
		TrapezoidSpeedProfile();
		virtual ~TrapezoidSpeedProfile();

		void init(double aa, double ad, double vc)
		{
			m_aa = aa;
			m_ad = ad;
			m_vc = vc;
			m_type = ProfileType::None;
		}

		void reset()
		{
			m_type = ProfileType::None;
		}

		virtual void calcProfile(double vs, double ve, double dist);
		virtual void calcProfile(double dist)
		{
			calcProfile(0.0, 0.0, dist);
		}

		ProfileType getType() const
		{
			return m_type;
		}

		std::string toString() const;

		virtual double getTargetSpeed(double t) const;
		virtual double getTargetDistance(double t) const;

		double getTA() const
		{
			return m_ta;
		}

		double getTC() const
		{
			return m_tc;
		}

		double getTD() const
		{
			return m_td;
		}

		double getVT() const
		{
			return m_vt;
		}
		
		virtual bool isDone(double time) const
		{
			return m_type != ProfileType::None && time > m_ta + m_tc + m_td;
		}

	private:
		double calcDistance() const;

	private:

		//
		// Characteristics of the robot
		//
		double m_aa;
		double m_ad;
		double m_vc;

		//
		// Desired characteristics of this speed profile
		//
		double m_dist;
		double m_ve;
		double m_vs;

		//
		// The result of calculating this profile
		//
		ProfileType m_type;
		double m_vt;
		double m_ta;
		double m_tc;
		double m_td;

		double m_da;
		double m_dc;
		double m_dd;
	};
}
