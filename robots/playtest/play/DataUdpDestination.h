#pragma once

#include "DataDestination.h"
#include "UdpBroadcastSender.h"

namespace xerolib
{
	class DataUdpDestination : public DataDestination
	{
	public:
		DataUdpDestination(uint16_t m_port);
		virtual ~DataUdpDestination();

		virtual void endRow(const std::vector<std::string> &names,
			const std::vector<double> &data, const std::vector<bool> &present);

		virtual void startDataSet(const std::vector<std::string> &names);


		virtual void endDataSet()
		{
		}

		virtual void startRow()
		{
		}

	private:
		bwgnet::UdpBroadcastSender m_sender;
		size_t m_seq_no;
	};
}
