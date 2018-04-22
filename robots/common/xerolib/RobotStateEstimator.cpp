#include "RobotStateEstimator.h"
#include "PositionAngle.h"
#include "RobotState.h"
#include "Kinematics.h"

using namespace xero::math;
using namespace xero::motion;

namespace xero
{
	namespace pathfinder
	{
		RobotStateEstimator RobotStateEstimator::TheOneObject;

		RobotStateEstimator::RobotStateEstimator()
		{
			m_left_dist_prev = 0.0;
			m_right_dist_prev = 0.0;
		}

		RobotStateEstimator::~RobotStateEstimator()
		{
		}

		void RobotStateEstimator::oneLoop(double t)
		{
			RobotState &st = RobotState::get();

			double left = m_db_p->getLeftDistance();
			double right = m_db_p->getRightDistance();
			Rotation angle = m_db_p->getAngle();

			PositionAngle od_vel = st.generateOdometryFromSensors(left - m_left_dist_prev, right - m_right_dist_prev, angle);
			PositionAngle predicted_vel = Kinematics::forwardKinematics(m_db_p->getLeftSpeed(), m_db_p->getRightSpeed(), m_db_p->getScrubFactor());
			st.addActualPosition(t, od_vel, predicted_vel);
			m_left_dist_prev = left;
			m_right_dist_prev = right;

			// std::cout << " Odometry Velocity:" << od_vel.toString() << std::endl;
			// std::cout << "Predicted Velocity: " << predicted_vel.toString() << std::endl;
		}
	}
}
