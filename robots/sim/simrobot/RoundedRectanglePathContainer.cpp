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

	std::string name = "RoundedRect(";
	name += std::to_string(straight);
	name += ", " + std::to_string(corner);
	name += ", " + std::to_string(speed);
	name += ")";
	setName(name);
}

RoundedRectanglePathContainer::~RoundedRectanglePathContainer()
{
}

std::shared_ptr<xero::pathfinder::Path> RoundedRectanglePathContainer::buildPath(double maxaccel)
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
	return xero::pathfinder::PathBuilder::buildPath(points, maxaccel);
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