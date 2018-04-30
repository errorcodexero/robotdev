#include "Pathfinder.h"

namespace xero
{
	namespace jacispath
	{
		Pathfinder::Pathfinder(const std::vector<Waypoint> &waypoints, FitFun fit, size_t count, double dt,
			double maxvel, double maxaccel, double maxjerk, TrajectoryCandidate &cand)
		{
			double totallength = 0.0;

			for (size_t i = 0; i < waypoints.size() - 1; i++)
			{
				std::shared_ptr<Spline> spline_p = std::make_shared<Spline>();
				m_splines.push_back(spline_p);

				fit(waypoints[i], waypoints[i + 1], *spline_p);

				double dist = spline_p->distance(count);
				m_lapts.push_back(dist);

				totallength += dist;
			}

			TrajectoryConfig config(dt, maxvel, maxaccel, maxjerk, 0, waypoints[0].getAngle(), 
				totallength, 0, waypoints[0].getAngle(), count);
			TrajectoryInfo info(config);

			m_total_length = totallength;
			m_traj_length = info.getLength();
			m_path_length = waypoints.size();

			m_info
		}

		Pathfinder::~Pathfinder()
		{
		}
	}
}
