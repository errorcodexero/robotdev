#include "AdaptivePurePursuitController.h"
#include <algorithm>

using namespace xero::math;

namespace xero
{
	namespace pathfinder
	{
		AdaptivePurePursuitController::Command AdaptivePurePursuitController::update(const PositionCS &p)
		{
			PositionCS pose(p);
			if (m_reversed)
				pose = PositionCS(p.getPos(), p.getRotation().rotateBy(Rotation::fromDegrees(180.0)));

			Path::TargetPointReport report = m_path_p->getTargetPoint(pose.getPos(), m_lookahead);
			if (isFinished())
			{
				PositionAngle pa;
				return Command(pa, report.ClosestPointDistance, report.MaxSpeed, 0.0, report.LookAheadPoint, report.RemainingPathDistance);
			}

			Arc arc(pose, report.LookAheadPoint);
			double scale_factor = 1.0;

			if (report.LookAheadPointSpeed < kDelta && report.RemainingPathDistance < arc.Length)
			{
				scale_factor = std::max(0.0, report.RemainingPathDistance / arc.Length);
				m_at_end = true;
			}
			else
			{
				m_at_end = false;
			}

			if (m_reversed)
				scale_factor *= -1.0;

			double angle = arc.Length * getDirection(pose, report.LookAheadPoint) * std::fabs(scale_factor) / arc.Radius;
			PositionAngle pa(scale_factor * arc.Length, 0.0, angle);
			return Command(pa, report.ClosestPointDistance, report.MaxSpeed, std::copysign(report.LookAheadPointSpeed, scale_factor), 
				report.LookAheadPoint, report.RemainingPathDistance);
		}
	}
}
