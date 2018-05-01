#pragma once

#include "Coord.h"

namespace xero
{
	namespace jacispath
	{
		class Segment
		{
		public:
			Segment(double dt, double x, double y, double pos, double vel, double accel, double jerk, double head)
			{
				m_dt = dt;
				m_x = x;
				m_y = y;
				m_position = pos;
				m_velocity = vel;
				m_acceleration = accel;
				m_jerk = jerk;
				m_heading = head;
			}

			void setHeading(double angle)
			{
				m_heading = angle;
			}

			void setPosition(const Coord &pos)
			{
				m_x = pos.getX();
				m_y = pos.getY();
			}

			double getDT() const
			{
				return m_dt;
			}

			double getX() const
			{
				return m_x;
			}

			double getY() const
			{
				return m_y;
			}

			double getPosition() const
			{
				return m_position;
			}

			double getVelocity() const
			{
				return m_velocity;
			}

			double getAcceleration() const
			{
				return m_acceleration;
			}

			double getJerk() const
			{
				return m_jerk;
			}

			double getHeading() const
			{
				return m_heading;
			}

		private:
			double m_dt;
			double m_x;
			double m_y;
			double m_position;
			double m_velocity;
			double m_acceleration;
			double m_jerk;
			double m_heading;
		};
	}
}