#include "LineArcLinePathContainer.h"
#include "PathBuilder.h"

using namespace xero::math;
using namespace xero::pathfinder;
using namespace xero::motion;

LineArcLinePathContainer::LineArcLinePathContainer()
{
	m_name = "CrossScale";
}

LineArcLinePathContainer::~LineArcLinePathContainer()
{
}

std::string LineArcLinePathContainer::getName() const
{
	return m_name;
}

std::shared_ptr<xero::pathfinder::Path> LineArcLinePathContainer::buildPath(double maxaccel)
{
	std::vector<Waypoint> points;

	points.push_back(Waypoint(0.0, 90, 0, 0));
	points.push_back(Waypoint(220, 90, 0, 80));
#ifdef NOTYET
	points.push_back(Waypoint(230, 90, 10, 80));
	points.push_back(Waypoint(240, 100, 10, 80));
	points.push_back(Waypoint(250, 130, 10, 80));
	points.push_back(Waypoint(250, 250, 0, 80));
	points.push_back(Waypoint(260, 260, 10, 80));
	points.push_back(Waypoint(270, 260, 10, 80));
	points.push_back(Waypoint(310, 260, 0, 80));
#endif
	return xero::pathfinder::PathBuilder::buildPath(points, maxaccel);
}

xero::math::PositionCS LineArcLinePathContainer::getStartPose()
{
	PositionCS ret(Position(0, 0), Rotation::fromDegrees(90));
	return ret;
}

bool LineArcLinePathContainer::isReversed()
{
	return false;
}