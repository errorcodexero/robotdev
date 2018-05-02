#pragma once

#include "Spline.h"
#include "TrajectoryInfo.h"
#include "TrajectoryConfig.h"
#include "Waypoint.h"
#include "Segment.h"
#include <memory>
#include <vector>
#include <functional>

namespace xero
{
	namespace jacispath
	{
		class TrajectoryCandidate
		{
		public:
			static constexpr size_t PATHFINDER_SAMPLES_FAST = 1000;
			static constexpr size_t PATHFINDER_SAMPLES_LOW = 10000;
			static constexpr size_t PATHFINDER_SAMPLES_HIGH = 100000;

		public:
			typedef std::function<void(const Waypoint &, const Waypoint &, Spline &)> FitFun;

		public:
			TrajectoryCandidate(const std::vector<Waypoint> &waypoints, FitFun fit, size_t count, double dt,
				double maxvel, double maxaccel, double maxjerk);
			virtual ~TrajectoryCandidate();

			void generate(std::vector<Segment> &segments);

		private:
			std::vector<std::shared_ptr<Spline>> m_splines;
			std::vector<double> m_lookahead;
			double m_total_length;
			int m_length;
			size_t m_path_length;
			size_t m_traj_length;
			TrajectoryInfo m_info;
			TrajectoryConfig m_config;
		};
	}
}
