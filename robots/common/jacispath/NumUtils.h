#pragma once
#include <algorithm>

namespace xero
{
	namespace jacispath
	{
		static double constexpr PI = 3.14159265359;

		double inline clamp(double value, double min, double max)
		{
			return std::min(std::max(min, value), max);
		}

		double inline bound_radians(double rad)
		{
			double ang = std::fmod(rad, PI * 2.0);
			if (ang < 0.0)
				ang += PI * 2.0 ;

			return ang;
		}
	}
}