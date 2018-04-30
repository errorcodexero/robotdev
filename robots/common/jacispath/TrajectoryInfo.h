#pragma once

#include "TrajectoryConfig.h"

namespace xero
{
	namespace jacispath
	{
		class TrajectoryInfo
		{
		public:
			TrajectoryInfo(const TrajectoryConfig &c);
			virtual ~TrajectoryInfo();

			int getLength() const
			{
				return m_length;
			}

		private:
			int m_filter_1;
			int m_filter_2;
			int m_length;
			double m_dt;
			double m_u;
			double m_v;
			double m_impulse;
		};
	}
}

