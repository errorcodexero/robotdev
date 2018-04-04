#include "PathFollower.h"

using namespace xero::math;
using namespace xero::motion;

namespace xero
{
	namespace pathfinder
	{
		PathFollower::PathFollower(std::shared_ptr<Path> path_p, bool reversed, const Parameters &params)
		{
			m_steering_controller_p = std::make_shared<AdaptivePurePursuitController>(path_p, reversed, params.lookahead);
			m_velocity_controller_p = std::make_shared<ProfileFollower>(params.profile_kp, params.profile_ki, params.profile_kv,
				params.profile_kffv, params.profile_kffa);
			m_velocity_controller_p->setConstraints(Constraints(params.profile_max_abs_vel, params.profile_max_abs_acc));
			m_max_profile_vel = params.profile_max_abs_vel;
			m_max_profile_acc = params.profile_max_abs_acc;
			m_goal_pos_tolerance = params.goal_pos_tolerance;
			m_goal_vel_tolerance = params.goal_vel_tolerance;
			m_inertia_gain = params.inertia_gain;
			m_stop_steering_distance = params.stop_steering_distance;
		}

		PathFollower::~PathFollower()
		{
		}

		PositionAngle PathFollower::update(double t, const xero::math::PositionCS &pose, double disp, double vel)
		{
			if (!m_steering_controller_p->isFinished())
			{
				AdaptivePurePursuitController::Command cmd = m_steering_controller_p->update(pose);
				m_debug_output.lookahead_point_x = cmd.m_lookahead_point.getX();
				m_debug_output.lookahead_point_y = cmd.m_lookahead_point.getY();
				m_debug_output.lookahead_point_velocity = cmd.m_end_velocity;
				m_debug_output.steering_command_dx = cmd.m_delta.getX();
				m_debug_output.steering_command_dy = cmd.m_delta.getY();
				m_debug_output.steering_command_dtheta = cmd.m_delta.getAngle();
				m_cross_track_error = cmd.m_cross_track_error;
				m_last_steering_delta = cmd.m_delta;

				ProfileGoal g(disp + cmd.m_delta.getX(), std::fabs(cmd.m_end_velocity), ProfileGoal::CompletionType::ExceedMaxAccel, m_goal_pos_tolerance, m_goal_vel_tolerance);
				Constraints c(std::min(m_max_profile_vel, cmd.m_max_velocity), m_max_profile_acc);
				m_velocity_controller_p->setGoalAndConstraints(g, c);

				if (cmd.m_remaining_path_length < m_stop_steering_distance)
					m_done_steering = true;
			}

			State st(t, disp, vel, 0.0);
			double velcmd = m_velocity_controller_p->update(st, t);
			m_along_track_error = m_velocity_controller_p->getPositionError();
			double curvature = m_last_steering_delta.getAngle() / m_last_steering_delta.getX();
			double dtheta = m_last_steering_delta.getAngle();

			if (!std::isnan(curvature) && std::fabs(curvature) < Constants::kMaxAdaptivePursuitArcRadius)
			{
				double abs_velocity_setpoint = std::fabs(m_velocity_controller_p->getSetpoint()->getState().getVelocity());
				dtheta = m_last_steering_delta.getX() * curvature * (1.0 + m_inertia_gain * abs_velocity_setpoint);
			}

			double scale = velcmd / m_last_steering_delta.getX() ;
			PositionAngle ret(m_last_steering_delta.getX() * scale, 0.0, dtheta * scale);

			m_debug_output.t = t;
			m_debug_output.pose_x = pose.getPos().getX();
			m_debug_output.pose_y = pose.getPos().getY();
			m_debug_output.pose_theta = pose.getRotation().getRadians();
			m_debug_output.linear_displacement = disp;
			m_debug_output.linear_velocity = vel;
			m_debug_output.profile_displacement = m_velocity_controller_p->getSetpoint()->getState().getPosition();
			m_debug_output.profile_velocity = m_velocity_controller_p->getSetpoint()->getState().getVelocity();
			m_debug_output.velocity_command_dx = ret.getX();
			m_debug_output.velocity_command_dy = ret.getY();
			m_debug_output.velocity_command_dtheta = ret.getAngle();
			m_debug_output.cross_track_error = m_cross_track_error;
			m_debug_output.along_track_error = m_along_track_error;

			return ret;
		}
	}
}
