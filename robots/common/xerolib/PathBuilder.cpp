#include "PathBuilder.h"
#include <cassert>

namespace xero
{
	namespace pathfinder
	{

		PathBuilder::PathBuilder()
		{
		}
		PathBuilder::~PathBuilder()
		{
		}

		std::shared_ptr<Path> PathBuilder::buildPath(const std::vector<Waypoint> &waypoints)
		{
			assert(waypoints.size() >= 2);
			std::shared_ptr<Path> path_p = std::shared_ptr<Path>();

			if (waypoints.size() > 2)
			{
				size_t i = 0;
				do {
					Arc a(waypoints[i], waypoints[i + 1], waypoints[i + 2]);
					a.addToPath(path_p);
				} while (i < waypoints.size() - 2);
			}

			Line l(waypoints[waypoints.size() - 2], waypoints[waypoints.size() - 1]);
			l.addToPath(path_p, 0);

			path_p->extrapolateLast();
			path_p->verifySpeeds();

			return path_p;
		}
	}
}
