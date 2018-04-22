#pragma once

#include "Rotation.h"
#include "PositionAngle.h"
#include "PositionCS.h"

namespace xero
{
	namespace motion
	{
		class Kinematics
		{
		public:
			Kinematics()
			{
			}

			virtual ~Kinematics()
			{
			}

			static xero::math::PositionAngle forwardKinematics(const xero::math::Rotation &prevrot,
				double ldist, double rdist, const xero::math::Rotation &gyro)
			{
				double dangle = prevrot.inverse().rotateBy(gyro).getRadians();
				return forwardKinematics(ldist, rdist, dangle);
			}

			static xero::math::PositionAngle forwardKinematics(double leftdelta, double rightdelta, double width, double scrub)
			{
				double dv = (rightdelta - leftdelta) / 2 * scrub;
				double dr = (dv * 2.0) / width;
				return forwardKinematics(leftdelta, rightdelta, dr);
			}

			static xero::math::PositionAngle forwardKinematics(double leftdelta, double rightdelta, double rot)
			{
				double dx = (leftdelta + rightdelta) / 2.0;
				return xero::math::PositionAngle(dx, 0, rot);
			}

			static xero::math::PositionCS integrateForwardKinematics(const xero::math::PositionCS &current, 
				const xero::math::PositionAngle &forward)
			{
				return current.transformBy(xero::math::PositionCS::exp(forward));
			}

			static void inverseKinematics(xero::math::PositionAngle const &pa, double width, double scrub, double &left, double &right)
			{
				if (std::fabs(pa.getAngle()) < 0)
				{
					left = pa.getX();
					right = pa.getX();
				}
				else
				{
					double dv = width * pa.getAngle() / (2 * scrub);
					left = pa.getX() - dv;
					right = pa.getX() + dv;
				}
			}
		};
	}
}
