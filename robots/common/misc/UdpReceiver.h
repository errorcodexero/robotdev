#pragma once

#include "UdpSocket.h"
#include <vector>
#include <string.h>

namespace xeromisc
{
	class UdpBroadcastReceiver : public UdpSocket
	{
	public:
		UdpBroadcastReceiver()
		{
		}

		virtual ~UdpBroadcastReceiver()
		{
		}

		bool open(const std::string &addr, uint16_t port)
		{
			if (!createSocket())
				return false;

			if (!bind(addr, port))
			{
				destroySocket();
				return false;
			}

			return true;
		}

		bool open(uint16_t port)
		{
			std::string empty;
			return open(empty, port);
		}

		int receive(std::vector<uint8_t> &data)
		{
			struct sockaddr_in srcaddr;
			size_t len = sizeof(srcaddr);
			int flags = 0;
			ssize_t count;

			std::cout << "Waiting For Packet, size is " << data.size() << std::endl;
			count = recvfrom(getSocket(), &data[0], data.size(), flags, (sockaddr *)&srcaddr, &len);
			std::cout << "RECEIVED" << std::endl;
			if (count == -1)
				return -1;

			return count;
		}

	protected:
		bool bind(const std::string &addr, uint16_t port)
		{
			std::cout << "Binding socket to address '" << addr << "'" << std::endl;
			memset(&m_saddr, 0, sizeof(m_saddr));
			m_saddr.sin_family = AF_INET;
			if (addr.length() == 0)
			{
				std::cout << "Binding to address INADDR_ANY" << std::endl;
				m_saddr.sin_addr.s_addr = INADDR_ANY;
			}
			else
			{
				if (inet_aton(addr.c_str(), &m_saddr.sin_addr) == 0)
				{
					destroySocket();
					return false;
				}
			}
			m_saddr.sin_port = htons(port);

			if (::bind(getSocket(), (struct sockaddr *)&m_saddr, sizeof(m_saddr)) == -1)
			{
				destroySocket();
				return false;
			}

			return true;
		}

	private:
		struct sockaddr_in m_saddr;
	};
}
