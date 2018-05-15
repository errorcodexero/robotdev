#include "Path.h"
#include "SegmentIO.h"

namespace xero
{
	namespace motion
	{
		Path::Path()
		{
		}

		Path::~Path()
		{
		}

		bool Path::create(const std::string &name, const std::string &filename)
		{
			m_name = name;

			if (!SegmentIO::readSegments(filename, m_segments))
				return false;

			return true;
		}
	}
}
