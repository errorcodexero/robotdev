#pragma once

#include <PathContainer.h>

class DriveSquarePathContainer : public xero::pathfinder::PathContainer
{
public:
	DriveSquarePathContainer();
	virtual ~DriveSquarePathContainer();

	virtual std::string getName() const;
	virtual std::shared_ptr<xero::pathfinder::Path> buildPath();
	virtual xero::math::PositionCS getStartPose();

	virtual bool isReversed()
	{
		return false;
	}

private:
	std::string m_name;
};

