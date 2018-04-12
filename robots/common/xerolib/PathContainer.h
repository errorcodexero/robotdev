#pragma once

#include <string>
#include <memory>
#include "PositionCS.h"
#include "Path.h"

namespace xero
{
	namespace pathfinder
	{
		class PathContainer
		{
		public:
			PathContainer()
			{
			}

			virtual ~PathContainer()
			{
			}

			virtual std::string getName() const = 0;
			virtual std::shared_ptr<Path> buildPath() = 0;
			virtual xero::math::PositionCS getStartPose() = 0;
			virtual bool isReversed() = 0;
		};
	}
}

