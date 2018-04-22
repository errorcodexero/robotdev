#include "RoundedRectanglePathContainer.h"
#include "PathBuilder.h"

using namespace xero::math;
using namespace xero::pathfinder;
using namespace xero::motion;

RoundedRectanglePathContainer::RoundedRectanglePathContainer(double straight, double corner, double speed)
{
	m_side = straight;
	m_radius = corner;
	m_speed = speed;
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

	points.push_back(Waypoint(0, m_radius, deg2rad(90.0), m_speed));
	points.push_back(Waypoint(0.0, m_side + m_radius, deg2rad(90.0), m_speed));
	points.push_back(Waypoint(m_radius, m_side + 2 * m_radius, deg2rad(0.0), m_speed));
	points.push_back(Waypoint(m_radius + m_side, m_side + 2 * m_radius, deg2rad(0.0), m_speed));
	points.push_back(Waypoint(m_radius + 2 * m_side, m_radius + m_side, deg2rad(-90.0), m_speed));
	points.push_back(Waypoint(m_radius + 2 * m_side, m_radius, deg2rad(-90.0), m_speed));
	points.push_back(Waypoint(m_radius + m_side, 0.0, deg2rad(180.0), m_speed));
	points.push_back(Waypoint(m_radius, 0.0, deg2rad(180.0), m_speed));
	points.push_back(Waypoint(0, m_radius, deg2rad(90.0), m_speed));
	return xero::pathfinder::PathBuilder::buildPath(points);
}

xero::math::PositionCS RoundedRectanglePathContainer::getStartPose()
{
	PositionCS ret(Position(0.0, 0.0), Rotation::fromDegrees(90.0));
	return ret;
}

bool RoundedRectanglePathContainer::isReversed()
{
	return false;
}