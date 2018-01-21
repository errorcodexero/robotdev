#pragma once

#include <iostream>
#include <cstdint>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace xerolib
{
	class TcpClientSocket
	{
	public:
		TcpClientSocket();
		~TcpClientSocket();

		bool open(const std::string &host, uint16_t port);
		bool write(const std::string &msg);

	private:
		int m_socket;
	};
}

