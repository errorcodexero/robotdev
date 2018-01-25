#include "DataTcpDestination.h"
#include <cassert>

namespace xerolib
{

	DataTcpDestination::DataTcpDestination(const std::string &addr, uint16_t port)
	{
		m_socket.open(addr, port);
		m_seq_no = 0;
	}

	DataTcpDestination::~DataTcpDestination()
	{
	}

	void DataTcpDestination::endRow(const std::vector<std::string> &names, const std::vector<double> &data, const std::vector<bool> &present)
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
		m_socket.write(line);
	}
}
