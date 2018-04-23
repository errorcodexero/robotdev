#include "StraightLinePathContainer.h"
#include "PathBuilder.h"

using namespace xero::math;
using namespace xero::pathfinder;
using namespace xero::motion;

StraightLinePathContainer::StraightLinePathContainer(double length, double speed)
{
	m_length = length;
	m_speed = speed;
	m_name = "StraightLine " + std::to_string(length) + " , " + std::to_string(speed);
}

StraightLinePathContainer::~StraightLinePathContainer()
{
}

std::string StraightLinePathContainer::getName() const
{
	return m_name;
}

std::shared_ptr<xero::pathfinder::Path> StraightLinePathContainer::buildPath(double accel)
{
	std::vector<Waypoint> points;

	points.push_back(Waypoint(0, 0, 0, m_speed));
	points.push_back(Waypoint(m_length, 0.0, 0.0, m_speed));

	return xero::pathfinder::PathBuilder::buildPath(points, accel);
}

xero::math::PositionCS StraightLinePathContainer::getStartPose()
{
	PositionCS ret;
	return ret;
}

bool StraightLinePathContainer::isReversed()
{
	return false;
}