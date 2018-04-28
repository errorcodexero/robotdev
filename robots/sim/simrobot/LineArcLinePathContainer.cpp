#include "LineArcLinePathContainer.h"
#include "PathBuilder.h"

using namespace xero::math;
using namespace xero::pathfinder;
using namespace xero::motion;

LineArcLinePathContainer::LineArcLinePathContainer() : PathContainer("CrossScale")
{
}

LineArcLinePathContainer::~LineArcLinePathContainer()
{
}

std::shared_ptr<xero::pathfinder::Path> LineArcLinePathContainer::buildPath(double maxaccel)
{
	std::vector<Waypoint> points;

	points.push_back(Waypoint(0, 90, 0, 0));
	points.push_back(Waypoint(10, 90, 0, 80));
	points.push_back(Waypoint(20, 90, 10, 80));
	points.push_back(Waypoint(30, 100, 10, 80));
	points.push_back(Waypoint(40, 130, 10, 80));
	points.push_back(Waypoint(40, 250, 0, 80));
	points.push_back(Waypoint(50, 260, 10, 80));
	points.push_back(Waypoint(60, 260, 10, 80));
	points.push_back(Waypoint(100, 260, 0, 80));

	return xero::pathfinder::PathBuilder::buildPath(points, maxaccel);
}

xero::math::PositionCS LineArcLinePathContainer::getStartPose()
{
	PositionCS ret(Position(0, 90), Rotation::fromDegrees(90));
	return ret;
}

bool LineArcLinePathContainer::isReversed()
{
	return false;
}