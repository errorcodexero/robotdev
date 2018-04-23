#include "RoundedRectanglePathContainer.h"
#include "PathBuilder.h"

using namespace xero::math;
using namespace xero::pathfinder;
using namespace xero::motion;

RoundedRectanglePathContainer::RoundedRectanglePathContainer()
{
	m_name = "RoundedRectangle";
}

RoundedRectanglePathContainer::~RoundedRectanglePathContainer()
{
}

std::string RoundedRectanglePathContainer::getName() const
{
	return m_name;
}

std::shared_ptr<xero::pathfinder::Path> RoundedRectanglePathContainer::buildPath()
{
	std::vector<Waypoint> points;

	points.push_back(Waypoint(0, 0, 0, 5));
	points.push_back(Waypoint(12.0, 0.0, 0.0, 5));

	return xero::pathfinder::PathBuilder::buildPath(points);
}

xero::math::PositionCS RoundedRectanglePathContainer::getStartPose()
{
	PositionCS ret;
	return ret;
}

bool RoundedRectanglePathContainer::isReversed()
{
	return false;
}