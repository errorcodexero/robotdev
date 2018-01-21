#include "DataLogger.h"
#include "DataFileDestination.h"

namespace xerolib
{
	const size_t DataLogger::badcol = std::numeric_limits<size_t>::max();

	DataLogger::DataLogger()
	{
		m_first = true;
	}

	DataLogger::~DataLogger()
	{
	}

	void DataLogger::addDestination(const std::string &filename)
	{
		std::shared_ptr<DataFileDestination> dest_p = std::make_shared<DataFileDestination>(filename);
		addDestination(dest_p);
	}
}
