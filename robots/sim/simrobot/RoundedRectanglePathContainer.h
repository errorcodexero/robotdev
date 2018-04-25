#pragma once

#include "PathContainer.h"

class RoundedRectanglePathContainer : public xero::pathfinder::PathContainer
{
public:
	RoundedRectanglePathContainer(double straight, double corner, double speed);
	~RoundedRectanglePathContainer();

	virtual std::shared_ptr<xero::pathfinder::Path> buildPath(double maxaccel);
	virtual xero::math::PositionCS getStartPose() ;
	virtual bool isReversed() ;

private:


private:
	double m_side;
	double m_radius;
	double m_speed;
};

