#include "DataFileDestination.h"
#include <cassert>

namespace xerolib
{
	DataFileDestination::DataFileDestination(const std::string &filename)
	{
		m_stream.open(filename);
	}

	DataFileDestination::~DataFileDestination()
	{
		m_stream.close();
	}

	void DataFileDestination::endRow(const std::vector<std::string> &names,
		const std::vector<double> &data, const std::vector<bool> &present)
	{
		for (size_t i = 0; i < names.size(); i++)
		{
			if (i != 0)
				m_stream << ",";

			if (present[i])
				m_stream << data[i];
		}
		m_stream << std::endl;
	}

	void DataFileDestination::startDataSet(const std::vector<std::string> &names)
	{
		for (size_t i = 0; i < names.size(); i++)
		{
			if (i != 0)
				m_stream << ',';

			m_stream << '"' << names[i] << '"';
		}
		m_stream << std::endl;
	}
}