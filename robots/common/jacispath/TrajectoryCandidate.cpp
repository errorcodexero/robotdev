#include "TrajectoryCandidate.h"

namespace xero
{
	namespace jacispath
	{
		TrajectoryCandidate::TrajectoryCandidate(const std::vector<Waypoint> &waypoints, TrajectoryCandidate::FitFun fit, size_t count, double dt,
			double maxvel, double maxaccel, double maxjerk)
		{
			double totallength = 0.0;

			for (size_t i = 0; i < waypoints.size() - 1; i++)
			{
				std::shared_ptr<Spline> spline_p = std::make_shared<Spline>();
				m_splines.push_back(spline_p);

				fit(waypoints[i], waypoints[i + 1], *spline_p);

				double dist = spline_p->distance(count);
				m_lookahead.push_back(dist);

				totallength += dist;
			}

			TrajectoryConfig config(dt, maxvel, maxaccel, maxjerk, 0, waypoints[0].getAngle(),
				totallength, 0, waypoints[0].getAngle(), count);
			TrajectoryInfo info(config);

			m_total_length = totallength;
			m_traj_length = info.getLength();
			m_path_length = waypoints.size();
			m_info = info;
			m_config = config;
		}

		TrajectoryCandidate::~TrajectoryCandidate()
		{
		}

		void TrajectoryCandidate::generate(std::vector<Segment> &segments)
		{
			segments.resize(m_traj_length);

			double spline_pos_initial = 0;
			double spline_complete = 0;
			size_t spline_i = 0;

			for (size_t i = 0; i < m_traj_length; i++)
			{
				double pos = segments[i].getPosition();
				bool found = false;
				while (!found)
				{
					double pos_relative = pos - spline_pos_initial;
					if (pos_relative <= m_lookahead[spline_i])
					{
						auto spline_p = m_splines[spline_i];
						double percentage = spline_p->progressForDistance(pos_relative, m_config.getCount());
						Coord coords = spline_p->coords(percentage);
						segments[i].setHeading(spline_p->angle(percentage));
						segments[i].setPosition(coords);
						found = true;
					}
					else if (spline_i < m_path_length - 2)
					{
						spline_complete += m_lookahead[spline_i];
						spline_pos_initial = spline_complete;
						spline_i++;
					}
					else
					{ 
						auto spline_p = m_splines[m_path_length - 2];
						segments[i].setHeading(spline_p->angle(1.0));
						segments[i].setPosition(spline_p->coords(1.0));
						found = true;
					}
				}
			}
		}
	}
}
