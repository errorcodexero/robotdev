#include "RedHopperPathContainer.h"
#include "PathBuilder.h"

using namespace xero::math;
using namespace xero::pathfinder;
using namespace xero::motion;

RedHopperPathContainer::RedHopperPathContainer() : PathContainer("RedHopperPath")
{
}

RedHopperPathContainer::~RedHopperPathContainer()
{
}

std::shared_ptr<xero::pathfinder::Path> RedHopperPathContainer::buildPath(double maxaccel)
{
	std::vector<Waypoint> points;

	points.push_back(Waypoint(120.775, 107.966, 0.0, 80.0));
	points.push_back(Waypoint(108.775, 87.181, 10.0, 80.0));
	points.push_back(Waypoint(143.44559465474475, 61.14542326662129, 30.0, 80.0));
	points.push_back(Waypoint(115.16132340728284, 32.8611520191594, 10.0, 80.0));
	points.push_back(Waypoint(84.500, 26.861, 0.0, 80.0));

	return xero::pathfinder::PathBuilder::buildPath(points, maxaccel);
}

xero::math::PositionCS RedHopperPathContainer::getStartPose()
{
	PositionCS ret(Position(0, 90), Rotation::fromDegrees(0.0));
	return ret;
}

bool RedHopperPathContainer::isReversed()
{
	return false;
}