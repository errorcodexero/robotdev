#pragma once

#include <vector>
#include <utility>
#include "PositionCS.h"
#include "PositionAngle.h"

namespace xero
{
	namespace pathfinder
	{
		class RobotState
		{
		private:
			RobotState();

		public:
			virtual ~RobotState();

			void reset(double start, const xero::math::PositionCS &pos);
			void reset(const xero::math::PositionCS &pos);

			void resetDrivenDistance()
			{
				m_driven_distance = 0;
			}

			static RobotState &getRobotState()
			{
				return TheOneObject;
			}

			xero::math::PositionCS getPositionAtTime(double t);
			double getDrivenDistance();
			double getPredictedVelocity();

			void addActualPosition(double t, const xero::math::PositionAngle &measured, const xero::math::PositionAngle &predicted);
			xero::math::PositionAngle generateOdometryFromSensors(double left_dist, double right_dist, const xero::math::Rotation &rot);

		private:
			size_t getBeforeIndex(double t);

		private:
			static RobotState TheOneObject;
			double m_driven_distance;
			double m_predicted_velocity;
			double m_time_window;
			std::vector<std::pair<double, xero::math::PositionCS>> m_actual_positions ;
		};
	}
}
