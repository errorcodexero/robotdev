#include "gtest/gtest.h"
#include <TrajectoryCandidate.h>
#include <Waypoint.h>
#include <NumUtils.h>
#include <Hermite.h>

using namespace xero::jacispath;

TEST(SimpleTest, Simple)
{
	std::vector<Segment> segs;

	std::vector<Waypoint> points = 
	{
		{ -4, -1, d2r(45) },
		{ -1, 2, 0},
		{ 2, 4, 0}
	};

	TrajectoryCandidate c(points, &Hermite::cubic, TrajectoryCandidate::PATHFINDER_SAMPLES_HIGH, 0.001, 15.0, 10.0, 60.0);
	c.generate(segs);
}
