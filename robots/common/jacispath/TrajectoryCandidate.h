#pragma once

#include "Spline.h"
#include "TrajectoryInfo.h"
#include "TrajectoryConfig.h"
#include <memory>
#include <vector>

namespace xero
{
	namespace jacispath
	{
		class TrajectoryCandidate
		{
		public:
			TrajectoryCandidate();
			virtual ~TrajectoryCandidate();

		private:
			std::shared_ptr<Spline> m_spline_p;
			std::vector<double> m_lookahead;
			double m_total_length;
			int m_length;
			int m_path_length;
			TrajectoryInfo m_info;
			TrajectoryConfig m_config;
		};
	}
}
