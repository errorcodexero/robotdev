#pragma once

#include "Pixy.h"

namespace Pixy {

// implementation class for serial UART
class PixyUART : public PixySerial
{
public:
	PixyUART( const std::string& device );
	virtual ~PixyUART();
	virtual bool open();
	virtual bool close();
	virtual bool isOpen() const;
	virtual int getByte();
	virtual int getWord();
	virtual int putString( const std::string& str );

private:
	std::string m_device;
	int m_fd;
};

}; // namespace Pixy

