#pragma once

#include "Waypoint.h"
#include <string>
#include <vector>

namespace xero
{
	namespace jacispath
	{

		class WaypointReader
		{
		public:
			WaypointReader() = delete;
			~WaypointReader() = delete;

			static bool readWaypoint(const std::string &filename, std::vector<Waypoint> &points);

		private:
			static std::vector<std::string> split(const std::string &line);
		};
	}
}

