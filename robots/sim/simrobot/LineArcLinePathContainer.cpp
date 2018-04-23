#include "LineArcLinePathContainer.h"
#include "PathBuilder.h"

using namespace xero::math;
using namespace xero::pathfinder;
using namespace xero::motion;

LineArcLinePathContainer::LineArcLinePathContainer()
{
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

#ifdef NOTYET
	points.push_back(Waypoint(0.0, 0.0, deg2rad(90.0), 24.0));
	points.push_back(Waypoint(-7.02944, 16.97056, deg2rad(135), 24.0));
	points.push_back(Waypoint(-24.00, 24.0, deg2rad(180.0), 24.0));
#else
	points.push_back(Waypoint(120.775, 107.966,  0.0, 80.0));
	points.push_back(Waypoint(108.775,  87.181, 10.0, 80.0));
	points.push_back(Waypoint(143.44559465474475, 61.14542326662129, 30.0, 80.0));
	points.push_back(Waypoint(115.16132340728284, 32.8611520191594, 10.0, 80.0));
	points.push_back(Waypoint( 84.500,  26.861,  0.0, 80.0));
#endif

	return xero::pathfinder::PathBuilder::buildPath(points, maxaccel);
}

xero::math::PositionCS LineArcLinePathContainer::getStartPose()
{
	PositionCS ret(Position(112, 115), Rotation::fromDegrees(0.0));
	return ret;
}

bool LineArcLinePathContainer::isReversed()
{
	return false;
}