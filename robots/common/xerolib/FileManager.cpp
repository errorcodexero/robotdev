#include "FileManager.h"
#include <ctime>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>

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
		if (access(name.c_str(), 0) == 0)
			return true;

		return false;
	}

	bool FileManager::is_directory(const std::string &name)
	{
		struct stat stbuf;

		if (stat(name.c_str(), &stbuf) == -1)
			return false;

		return (stbuf.st_mode & S_IFDIR) != 0;
	}

	bool FileManager::has_extension(const std::string &name)
	{
		size_t dot = name.find_last_of('.');
		return dot != std::string::npos && dot != name.length() - 1;
	}

	std::string FileManager::extension(const std::string &name)
	{
		size_t dot = name.find_last_of('.');
		return name.substr(dot + 1);
	}

	std::string FileManager::replace_extension(const std::string &name, const std::string &ext)
	{
		std::string result;

		size_t dot = name.find_last_of('.');
		if (dot == std::string::npos)
			return name + ext;

		return name.substr(0, dot + 1) + ext;
	}
}
