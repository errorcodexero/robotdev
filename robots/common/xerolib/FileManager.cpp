#include "FileManager.h"
#include <ctime>
#include <iostream>

namespace xerolib
{
	std::string FileManager::FlashDriveName("/media/sda1") ;

    FileManager::FileManager()
    {
		m_basedir = "/home/lvuser" ;
	
		const char *home_p = getenv("HOME") ;
		if (home_p != nullptr)
			m_basedir = home_p ;

		m_includetime = false ;
    
		if (exists(FlashDriveName) && is_directory(FlashDriveName))
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
		std::string filename = m_basedir + "/" + basename_p ;
	
		if (m_includetime)
		{
			std::string tfilename = filename ;
			time_t rawtime ;
			struct tm *timeinfo ;

			std::string ext ;

			if (has_extension(tfilename))
			{
				ext = extension(tfilename) ;
				tfilename = replace_extension(tfilename, "") ;
			}
	    
			time(&rawtime) ;
			timeinfo = localtime(&rawtime) ;
			strftime(&m_buffer[0], m_buffer.size(), "-%m-%d-%y-%H-%M-%S", timeinfo) ;

			filename = FlashDriveName ;
			filename += "/" + tfilename ;
			filename += &m_buffer[0];
			filename += ext ;
		}
		
		return filename;
    }

	bool FileManager::exists(const std::string &name)
	{
	}

	bool FileManager::is_directory(const std::string &name)
	{
	}
}
