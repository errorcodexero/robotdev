#pragma once

#include <experimental/filesystem>
#include <string>

namespace fs = std::experimental::filesystem ;

namespace xerolib
{
    class FileManager
    {
    public:
	FileManager() ;
	virtual ~FileManager() ;

	std::string makeFileName(const char *basename_p) ;

    public:
	static fs::path FlashDriveName ;

    private:
	fs::path m_basedir ;
	bool m_includetime ;
	std::array<char, 64> m_buffer ;
    } ;
}
