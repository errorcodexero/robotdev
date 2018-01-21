#include "DataUdpDestination.h"
#include <cassert>

namespace xerolib
{

	DataUdpDestination::DataUdpDestination(uint16_t port)
	{
		m_sender.open("10.70.74.149", port);
		m_seq_no = 0;
	}

	DataUdpDestination::~DataUdpDestination()
	{
	}

	void DataUdpDestination::startDataSet(const std::vector<std::string> &names)
	{
		m_seq_no = 0;
		m_sender.send("new");
	}

	void DataUdpDestination::endRow(const std::vector<std::string> &names, const std::vector<double> &data, const std::vector<bool> &present)
	{
		assert(names.size() == data.size());

		std::string line("data,");
		line += std::to_string(m_seq_no++);
		for (size_t i = 0; i < names.size(); i++)
		{
			line += ",";
			line += names[i];
			line += "=";
			double value = data[i];
			if (!present[i])
				value = 0.0;

			line += std::to_string(value);
		}
		m_sender.send(line);
	}
}
