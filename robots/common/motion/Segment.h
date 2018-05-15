#pragma once

#include "Position.h"

namespace xero
{
	namespace motion
	{
		class Segment
		{
		public:
			Segment()
			{
				m_dt = 0.0;
				m_x = 0.0;
				m_y = 0.0;
				m_position = 0.0;
				m_velocity = 0.0;
				m_acceleration = 0.0;
				m_jerk = 0.0;
				m_heading = 0.0;
			}

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

			void setDT(double t)
			{
				m_dt = t;
			}

			void setHeading(double angle)
			{
				m_heading = angle;
			}

			void setCoords(const Position &pos)
			{
				m_x = pos.getX();
				m_y = pos.getY();
			}

			void setPosition(double p)
			{
				m_position = p;
			}

			void setAcceleration(double a)
			{
				m_acceleration = a;
			}

			void setVelocity(double v)
			{
				m_velocity = v;
			}

			void setJerk(double j)
			{
				m_jerk = j;
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