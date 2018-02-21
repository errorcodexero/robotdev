#pragma once

#include <string>
#include <array>

namespace xerolib
{
	class FileManager
	{
	public:
		FileManager();
		virtual ~FileManager();

		std::string makeFileName(const char *basename_p);

	protected:
		static bool exists(const std::string &name);
		static bool is_directory(const std::string &name);
		static bool has_extension(const std::string &name);
		static std::string extension(const std::string &name);
		static std::string replace_extension(const std::string &name, const std::string &ext);

	public:
		static std::string FlashDriveName;

	private:
		std::string m_basedir;
		bool m_includetime;
		std::array<char, 64> m_buffer;
	};
}
