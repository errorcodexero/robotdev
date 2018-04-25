#pragma once

#include "PathContainer.h"

class LineArcLinePathContainer : public xero::pathfinder::PathContainer
{
public:
	LineArcLinePathContainer();
	~LineArcLinePathContainer();

	virtual std::shared_ptr<xero::pathfinder::Path> buildPath(double maxaccel);
	virtual xero::math::PositionCS getStartPose();
	virtual bool isReversed();

private:
};

