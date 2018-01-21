#include "ParamFileReader.h"
#include "MessageLogger.h"
#include <fstream>
#include <iostream>
#include <vector>

namespace xerolib
{
	ParamFileReader::ParamFileReader()
	{
	}

	ParamFileReader::~ParamFileReader()
	{
	}

	void ParamFileReader::split(const std::string &line, std::vector<std::string> &words)
	{
		size_t index = 0;
		std::string word;

		words.clear();
		while (index < line.length())
		{
			if (std::isspace(line[index]))
			{
				if (word.length() > 0)
				{
					words.push_back(word);
					word.clear();
				}
			}
			else
			{
				word += line[index];
			}
			index++;
		}

		if (word.length() > 0)
			words.push_back(word);
	}

	bool ParamFileReader::readParamFile(MessageLogger &logger, const std::string &filename, std::map<std::string, double> &values)
	{
		std::string line;
		std::vector<std::string> words;
		size_t idx;
		size_t lineno = 0;

		std::ifstream infile(filename);
		if (infile.bad() || infile.fail())
			return false;

		while (std::getline(infile, line))
		{
			std::cout << "line: " << line << std::endl ;
			size_t index = line.find('#');
			if (index != std::string::npos)
				line = line.substr(0, index);

			lineno++;
			if (line.length() == 0)
				continue;

			split(line, words);

			if (words.size() == 2)
			{
				double value;
				
				try
				{
					value = std::stod(words[1], &idx);
					if (idx == words[1].length())
					{
						values[words[0]] = value;
					}
					else
					{
						logger << MessageLogger::MessageType::Error << filename << ":" << lineno << ": ";
						logger << "invalid floating point number '" << words[1] << "' - trailing characters";
						logger << MessageLogger::Token::EndOfMessage;
					}
				}
				catch (const std::exception &ex)
				{
					logger << MessageLogger::MessageType::Error << filename << ":" << lineno << ": ";
					logger << "invalid floating point number '" << words[1] << "' - invalid number" ;
					logger << MessageLogger::Token::EndOfMessage;
				}
			}
			else if (words.size() != 0)
			{
				logger << MessageLogger::MessageType::Error << filename << ":" << lineno << ": ";
				logger << "invalid parameter line, '" << line << "' - expected two items seperated by a space";
				logger << MessageLogger::Token::EndOfMessage;
			}
		}

		infile.close();
		return true;
	}
}
