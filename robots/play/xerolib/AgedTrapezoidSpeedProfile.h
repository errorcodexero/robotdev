#pragma once
#include "TrapezoidSpeedProfile.h"
#include <stdexcept>

namespace xerolib
{
	class AgedTrapezoidSpeedProfile : public TrapezoidSpeedProfile
	{
	public:
		AgedTrapezoidSpeedProfile()
		{
		}

		virtual ~AgedTrapezoidSpeedProfile()
		{
		}

		void setMaxAge(double a)
		{
			m_max_age = a;
		}

		bool calcProfile(double time, double vs, double ve, double dist)
		{
			bool ret = false ;

			if (getType() == ProfileType::None || (time - m_create_time) > m_max_age)
			{
				TrapezoidSpeedProfile::calcProfile(vs, ve, dist);
				m_create_time = time;
				ret = true;
			}

			return ret;
		}

		bool calcProfile(double time, double dist)
		{
			return calcProfile(time, 0.0, 0.0, dist);
		}

		virtual void calcProfile(double vs, double ve, double dist)
		{
			std::string msg("it is illegal to call this method");
			std::runtime_error err(msg);
			throw err;
		}

		virtual void calcProfile(double dist)
		{
			std::string msg("it is illegal to call this method");
			std::runtime_error err(msg);
			throw err;
		}

		virtual double getTargetSpeed(double time)
		{
			return TrapezoidSpeedProfile::getTargetSpeed(time - m_create_time);
		}

		virtual double getTargetDistance(double time) const
		{
			return TrapezoidSpeedProfile::getTargetDistance(time - m_create_time);
		}

		virtual bool isDone(double time) const
		{
			return TrapezoidSpeedProfile::isDone(time - m_create_time);
		}

	private:
		double m_create_time;
		double m_max_age;
	};
}