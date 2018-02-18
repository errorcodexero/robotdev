#include "TcpClientSocket.h"
#include <string.h>
#include <iostream>

namespace xeromisc
{
    TcpClientSocket::TcpClientSocket()
    {
    }

    TcpClientSocket::~TcpClientSocket()
    {
    }

    bool TcpClientSocket::open(const std::string &addr, uint16_t port)
    {
	struct sockaddr_in saddr;

	memset(&saddr, 0, sizeof(saddr));

	if (!inet_aton(addr.c_str(), &saddr.sin_addr))
	    return false;

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);

	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket < 0)
	    return false;

	if (connect(m_socket, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
	{
	    close(m_socket);
	    return false;
	}

	return true;
    }

    bool TcpClientSocket::write(const std::string &amsg)
    {
	std::string msg = amsg + "\n";
	const char *data_p = msg.c_str();
	size_t length = msg.length();

	while (length > 0)
	{
	    int n = ::write(m_socket, data_p, msg.length());
	    if (n < 0)
	    {
		return false;
	    }

	    data_p += n;
	    length -= n;
	}

	return true;
    }
}
