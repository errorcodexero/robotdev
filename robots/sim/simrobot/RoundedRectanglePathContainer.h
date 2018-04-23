#pragma once

#include "PathContainer.h"

class RoundedRectanglePathContainer : public xero::pathfinder::PathContainer
{
public:
	RoundedRectanglePathContainer(double straight, double corner, double speed);
	~RoundedRectanglePathContainer();

	virtual std::string getName() const ;
	virtual std::shared_ptr<xero::pathfinder::Path> buildPath(double maxaccel);
	virtual xero::math::PositionCS getStartPose() ;
	virtual bool isReversed() ;

private:
	double deg2rad(double deg)
	{
		return deg / 180.0 * PI;
	}
	static constexpr double PI = 3.1415926;

private:
	double m_side;
	double m_radius;
	double m_speed;
	std::string m_name;
};

