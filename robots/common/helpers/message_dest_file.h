#pragma once

#include "message_logger_dest.h"
#include <fstream>
#include <string>

class messageDestFile : public messageLoggerDest
{
public:
    messageDestFile(const std::string &name)
    {
	m_strm_p = new std::ofstream(name) ;
	if (!m_strm_p->is_open())
	{
	    std::cerr << "counld not open log file '" << name << "' for writing" << std::endl ;
	    m_strm_p = nullptr ;
	}
    }
    
    virtual ~messageDestFile()
    {
	if (m_strm_p != nullptr)
	    delete m_strm_p ;
    }

    virtual void displayMessage(const messageLogger::messageType &type, uint64_t subs, const std::string &msg)
    {
	if (m_strm_p != nullptr)
	    (*m_strm_p) << msg << std::endl ;
    }

private:
    std::ofstream *m_strm_p ;
} ;
