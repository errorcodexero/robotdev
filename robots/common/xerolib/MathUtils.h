#pragma once

#include "Position.h"
#include "Rotation.h"
#include <algorithm>

namespace xero
{
	namespace math
	{
		inline Rotation getAngle(const Position &a, const Position &b)
		{
			Rotation ret;

			double denom = a.getNorm() * b.getNorm();
			if (denom != 0)
			{
				double cosangle = Position::dot(a, b) / denom;
				double d = std::min(1.0, std::max(cosangle, -1.0));
				ret = Rotation::fromRadians(std::acos(d));
			}

			return ret;
		}

		inline Rotation getDirection(const Position &pos)
		{
			return Rotation(pos.getX(), pos.getY());
		}

		inline double getSign(double v)
		{
			if (v < 0.0)
				return -1.0;
			else if (v > 0.0)
				return 1.0;

			return 0.0;
		}
	}
}