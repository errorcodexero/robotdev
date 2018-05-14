// jacipathgen.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <TrajectoryCandidate.h>
#include <Waypoint.h>
#include <WaypointReader.h>
#include <SegmentWriter.h>
#include <NumUtils.h>
#include <Hermite.h>
#include <Segment.h>

using namespace xero::jacispath;
using namespace xero::motion;

int main(int ac, char **av)
{
	ac--;
	av++;

	if (ac != 2)
	{
		std::cerr << "jacipathgen: input_file output_file" << std::endl;
		return 1;
	}

	std::vector<Waypoint> points;
	if (!WaypointReader::readWaypoint(av[0], points))
	{
		std::cerr << "jacipathgen: error reading waypoints" << std::endl;
		return 1;
	}

	std::vector<Segment> segs;
	TrajectoryCandidate c(points, &Hermite::cubic, TrajectoryCandidate::PATHFINDER_SAMPLES_LOW, 0.05, 15.0, 10.0, 60.0);
	if (c.isValid())
	{
		c.generate(segs);

		if (!SegmentWriter::writeSegments(av[1], segs))
		{
			std::cerr << "jacipathgen: error writing waypoints" << std::endl;
			return 1;
		}
	}
	else
	{
		std::cerr << "jacipathgen: invalid set of waypoints, cannot create path" << std::endl;
	}

	return 0;
}

