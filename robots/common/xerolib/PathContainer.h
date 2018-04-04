#pragma once

#include <string>
#include <memory>
#include "PositionCS.h"

namespace xero
{
	namespace pathfinder
	{
		class Path;

		class PathContainer
		{
		public:
			PathContainer(const char *name_p)
			{
				m_name = name_p;
			}

			virtual ~PathContainer()
			{
			}

			virtual std::string &getName() const
			{
				return m_name;
			}

			virtual std::shared_ptr<Path> buildPath() = 0;
			virtual xero::math::PositionCS getStartPose() = 0;
			virtual bool isReversed() = 0;

		private:
			std::string m_name;
		};
	}
}

