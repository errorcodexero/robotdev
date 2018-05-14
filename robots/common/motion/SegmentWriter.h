#pragma once

#include "Segment.h"
#include <vector>
#include <string>

namespace xero
{
	namespace motion
	{
		class SegmentWriter
		{
		public:
			SegmentWriter() = delete;
			~SegmentWriter() = delete;

			static bool writeSegments(const std::string &filename, const std::vector<xero::motion::Segment> &segments);
		};
	}
}
