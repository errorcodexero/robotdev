#pragma once

namespace xero
{
	namespace motion
	{
		class Coord
		{
		public:
			Coord()
			{
				m_x = 0.0;
				m_y = 0.0;
			}

			Coord(double x, double y)
			{
				m_x = x;
				m_y = y;
			}

			virtual ~Coord()
			{
			}

			double getX() const
			{
				return m_x;
			}

			double getY() const
			{
				return m_y;
			}

		private:
			double m_x;
			double m_y;
		};

		inline Coord operator+(const Coord &a, const Coord &b)
		{
			Coord result(a.getX() + b.getX(), a.getY() + b.getY());
			return result;
		}
	}
}
