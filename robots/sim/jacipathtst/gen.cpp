#include "gtest/gtest.h"
#include <TrajectoryCandidate.h>
#include <Waypoint.h>
#include <vector>

using namespace xero::jacispath;

#define K_PI 3.1415926

TEST(SimpleTest, Simple)
{
	std::vector<Waypoint> points = 
	{
		{ -4, -1, 45.0 / 180.0 * K_PI },
		{ -1, 2, 0},
		{2, 4, 0}
	};

	TrajectoryCandidate cand(points, fit, PATHERFINDER_SAMPLES_HIGH, 0.001, 15.0, 10.0, 60.0);
}
