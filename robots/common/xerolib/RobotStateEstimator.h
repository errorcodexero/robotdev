#pragma once

#include "DriveBase.h"

namespace xero
{
	namespace pathfinder
	{
		class RobotStateEstimator
		{
		public:
			RobotStateEstimator();
			~RobotStateEstimator();

			static RobotStateEstimator &get()
			{
				return TheOneObject;
			}

			void setDriveBase(xerolib::DriveBase *db_p)
			{
				m_db_p = db_p;
			}

			void oneLoop(double t);

		private:
			static RobotStateEstimator TheOneObject;

			double m_left_dist_prev;
			double m_right_dist_prev;

			xerolib::DriveBase *m_db_p;
		};
	}
}

