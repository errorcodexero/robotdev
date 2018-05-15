#pragma once

#include "Segment.h"
#include <vector>
#include <string>

namespace xero
{
	namespace motion
	{
		class Path
		{
		public:
			Path();
			~Path();

			const std::string &getName() const
			{
				return m_name;
			}

			bool create(const std::string &name)
			{
				m_name = name;
				return true;
			}

			bool create(const std::string &name, const std::string &filename);

			void addSegment(const Segment &seg)
			{
				m_segments.push_back(seg);
			}

			size_t size() const
			{
				return m_segments.size();
			}

			const Segment &operator[](size_t index)
			{
				return m_segments[index];
			}

		private:
			std::string m_name;
			std::vector<Segment> m_segments;
		};
	}
}

