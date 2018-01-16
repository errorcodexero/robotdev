#pragma once

#include <string>
#include <map>
#include <vector>

namespace xerolib
{
	class MessageLogger;

	class ParamFileReader
	{
	private:
		ParamFileReader();
		~ParamFileReader();

		static void split(const std::string &line, std::vector<std::string> &words);

	public:
		static bool readParamFile(MessageLogger &logger, const std::string &filename, std::map<std::string, double> &values);
	};
}

