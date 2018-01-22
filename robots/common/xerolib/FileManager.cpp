#include "FileManager.h"
#include <ctime>
#include <iostream>

namespace xerolib
{
    std::string FileManager::FlashDriveName("/media/sda1") ;


    FileManager::FileManager()
    {
	m_basedir = getenv("HOME") ;
	m_includetime = false ;
    
	if (fs::exists(FlashDriveName) && fs::is_directory(FlashDriveName))
	{
	    m_basedir = FlashDriveName ;
	    m_includetime = true ;
	}
    }

    FileManager::~FileManager()
    {
    }

    std::string FileManager::makeFileName(const char *basename_p)
    {
	fs::path filename = basename_p ;
	
	if (m_includetime)
	{
	    fs::path tfilename = filename ;
	    time_t rawtime ;
	    struct tm *timeinfo ;

	    std::string ext ;

	    if (tfilename.has_extension())
	    {
		ext = tfilename.extension() ;
		tfilename = tfilename.replace_extension("") ;
	    }
	    
	    time(&rawtime) ;
	    timeinfo = localtime(&rawtime) ;
	    strftime(&m_buffer[0], m_buffer.size(), "-%m-%d-%y-%H-%M-%S", timeinfo) ;

	    filename = FlashDriveName ;
	    filename /= tfilename ;
	    filename += &m_buffer[0];
	    filename += ext ;
	}
	return filename.generic_string() ;
    }
}
