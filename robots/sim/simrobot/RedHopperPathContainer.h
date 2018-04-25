#pragma once

#include <PathContainer.h>

class RedHopperPathContainer : public xero::pathfinder::PathContainer
{
public:
	RedHopperPathContainer();
	~RedHopperPathContainer();

	virtual std::shared_ptr<xero::pathfinder::Path> buildPath(double maxaccel);
	virtual xero::math::PositionCS getStartPose();
	virtual bool isReversed();

private:
};

