#pragma once

#include "PathContainer.h"

class StraightLinePathContainer : public xero::pathfinder::PathContainer
{
public:
	StraightLinePathContainer(double length, double speed);
	~StraightLinePathContainer();

	virtual std::shared_ptr<xero::pathfinder::Path> buildPath(double maxaccel);
	virtual xero::math::PositionCS getStartPose();
	virtual bool isReversed();

private:
	double m_speed;
	double m_length;
};

