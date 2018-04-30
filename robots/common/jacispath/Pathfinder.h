#pragma once

#include "Waypoint.h"
#include "Spline.h"
#include "TrajectoryCandidate.h"
#include <vector>
#include <functional>

namespace xero
{
	namespace jacispath
	{
		class Pathfinder
		{
		public:
			typedef std::function<void(const Waypoint &, const Waypoint &, Spline &)> FitFun;

		public:
			Pathfinder(const std::vector<Waypoint> &waypoints, FitFun fit, size_t count, double dt, 
				double maxvel, double maxaccel, double maxjerk, TrajectoryCandidate &cand);
			~Pathfinder();

		private:
			std::vector<std::shared_ptr<Spline>> m_splines;
			std::vector<double> m_lapts;
			double m_total_length;
			double m_traj_length;
			size_t m_path_length;
		};
	}
}


