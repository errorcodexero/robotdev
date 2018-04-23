#pragma once

#include "PathContainer.h"

class LineArcLinePathContainer : public xero::pathfinder::PathContainer
{
public:
	LineArcLinePathContainer();
	~LineArcLinePathContainer();

	virtual std::string getName() const;
	virtual std::shared_ptr<xero::pathfinder::Path> buildPath(double maxaccel);
	virtual xero::math::PositionCS getStartPose();
	virtual bool isReversed();

private:
	std::string m_name;
};

