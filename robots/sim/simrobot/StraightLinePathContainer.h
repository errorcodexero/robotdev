#pragma once

#include "PathContainer.h"

class StraightLinePathContainer : public xero::pathfinder::PathContainer
{
public:
	StraightLinePathContainer(double length, double speed);
	~StraightLinePathContainer();

	virtual std::string getName() const;
	virtual std::shared_ptr<xero::pathfinder::Path> buildPath(double maxaccel);
	virtual xero::math::PositionCS getStartPose();
	virtual bool isReversed();

private:
	std::string m_name;
	double m_speed;
	double m_length;
};

