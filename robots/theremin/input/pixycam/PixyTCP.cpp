#include "PixyTCP.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>

namespace Pixy {

PixyTCP::PixyTCP( const std::string& server, const std::string& service ): m_server(server), m_service(service), m_ai(NULL), m_fd(-1)
{
	;
}

PixyTCP::~PixyTCP()
{
	close();

	if (m_ai) {
		freeaddrinfo(m_ai);
		m_ai = NULL;
	}
}

bool PixyTCP::open()
{
	if (isOpen()) {
		return true;
	}

#ifdef TRACE
	std::cerr << "PixyTCP::open( \"" << m_server << "\", " << m_service << " )" << std::endl;
#endif

	if (!m_ai) {
		int err = getaddrinfo(m_server.c_str(), m_service.c_str(), NULL, &m_ai);
		if (err != 0) {
			fprintf(stderr, "PixyTCP::getaddrinfo: %s\n", gai_strerror(err));
			return false;
		}
	}

	m_fd = ::socket(m_ai->ai_family, m_ai->ai_socktype, m_ai->ai_protocol);
	if (m_fd == -1) {
		perror("socket");
		return false;
	}

	if (connect(m_fd, m_ai->ai_addr, m_ai->ai_addrlen) != 0) {
		perror("connect");
		close();
		return false;
	}

#ifdef TRACE
	std::cerr << "PixyTCP::open OK, fd = " << m_fd << std::endl;
#endif
	return true;
}

bool PixyTCP::close()
{
#ifdef TRACE
	std::cerr << "PixyTCP::close" << std::endl;
#endif

	if (isOpen()) {
		::close(m_fd);
		m_fd = -1;
	}
	return true;
}

bool PixyTCP::isOpen() const
{
	return (m_fd != -1);
}

int PixyTCP::getByte()
{
	if (!open()) {
		return -1;
	}

	uint8_t b;
	int n;
	do {
		n = read(m_fd, &b, 1);
		if (n == -1) {
			perror("PixyTCP::getByte");
			close();
			return -1;
		}
	} while (n != 1);
	char buf[3];
	snprintf(buf, 3, "%02x", b);
#ifdef TRACE
	std::cerr << " " << buf;
#endif
	return b;
}

int PixyTCP::getWord()
{
	if (!open()) {
		return -1;
	}

	int low, high;
	if ((low = getByte()) == -1) {
		return -1;
	}
	if ((high = getByte()) == -1) {
		return -1;
	}
	return (high << 8) | low;
}

int PixyTCP::putString( const std::string& str )
{
	if (!isOpen()) {
		return -1;
	}

	int n = write(m_fd, str.c_str(), str.size());
	if (n == -1) {
		perror("PixyTCP::write");
		close();
	}
	return n;
}

}; // namespace Pixy

