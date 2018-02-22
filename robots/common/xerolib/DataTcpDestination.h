#pragma once

#include "DataDestination.h"
#include "TcpClientSocket.h"

namespace xerolib
{
	class DataTcpDestination : public DataDestination
	{
	public:
		DataTcpDestination(const std::string &ipaddr, uint16_t port);
		~DataTcpDestination();

		virtual void endRow(const std::vector<std::string> &names,
			const std::vector<double> &data, const std::vector<bool> &present);

		virtual void startDataSet(const std::vector<std::string> &names)
		{
			m_socket.write("new\n", 4);
			m_seq_no = 0;
		}

		virtual void endDataSet()
		{
		}

		virtual void startRow()
		{
		}

	private:
		xeromisc::TcpClientSocket m_socket;
		size_t m_seq_no;
	};
}
