#pragma once

#include "Coord.h"
#include <vector>

namespace xero
{
	namespace jacispath
	{
		class Spline
		{
		public:
			Spline();
			~Spline();

			Coord coords(double per);
			double deriv(double per);
			double deriv(const std::vector<double> &coeffs, double k, double p);
			double angle(double per);
			double distance(size_t count);
			double progressForDistance(double dist, size_t count);

		private:
			static constexpr size_t a = 0;
			static constexpr size_t b = 1;
			static constexpr size_t c = 2;
			static constexpr size_t d = 3;
			static constexpr size_t e = 4;

		private:
			std::vector<double> m_coeffs;
			double m_knot_distance;
			double m_angle_offset;
			double m_arc_length;
			Coord m_offset;
		};
	}
}

