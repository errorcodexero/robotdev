#pragma once

#include "Pixy.h"
#include <netdb.h>

namespace Pixy {

// implementation class for serial UART
class PixyTCP : public PixySerial
{
public:
	PixyTCP( const std::string& host, const std::string& service );
	virtual ~PixyTCP();
	virtual bool open();
	virtual bool close();
	virtual bool isOpen() const;
	virtual int getByte();
	virtual int getWord();
	virtual int putString( const std::string& str );

private:
	std::string m_server;
	std::string m_service;
	struct addrinfo *m_ai;
	int m_fd;
};

}; // namespace Pixy

