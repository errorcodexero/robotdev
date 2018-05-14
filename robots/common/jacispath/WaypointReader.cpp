#include "WaypointReader.h"
#include "NumUtils.h"
#include <iostream>
#include <fstream>
#include <cctype>

namespace xero
{
	namespace jacispath
	{
		std::vector<std::string> WaypointReader::split(const std::string &line)
		{
			std::vector<std::string> ret;
			std::string word;

			size_t i = 0;
			while (i < line.length())
			{
				//
				// Skip leading spaces
				//
				while (i < line.length() && std::isspace(line[i]))
					i++;

				if (i == line.length())
					break;

				//
				// Capture the next wore
				//
				word.clear();
				while (i < line.length() && !std::isspace(line[i]))
					word += line[i++];

				//
				// Push the word onto the result
				//
				ret.push_back(word);
			}

			return ret;
		}

		bool WaypointReader::readWaypoint(const std::string &filename, std::vector<Waypoint> &points)
		{
			std::string line;

			std::ifstream in(filename);
			if (in.bad() || in.fail())
			{
				std::cerr << "cannot open file '" << filename << "' for reading" << std::endl;
				return false;
			}

			while (std::getline(in, line))
			{
				std::vector<std::string> words = split(line);
				if (words.size() == 0)
					continue;

				if (words.size() != 3)
				{
					std::cerr << "line: '" << line << "'" << std::endl;
					std::cerr << "      expected three words" << std::endl;
					points.clear();
					return false;
				}

				std::vector<double> values;
				for (size_t i = 0; i < words.size(); i++)
				{
					size_t end;
					double x = std::stod(words[i], &end);
					if (end != words[i].length())
					{
						std::cerr << "line: '" << line << "'" << std::endl;
						std::cerr << "      expected all floating point values" << std::endl;
						std::cerr << "      word '" << words[i] << "' is not a valid floating point number" << std::endl;
						points.clear();
						return false;
					}

					values.push_back(x);
				}

				double a = values[2] / 180 * PI ;
				a = bound_radians(a);
				Waypoint pt(values[0], values[1], a);
				points.push_back(pt);
			}

			return true;
		}
	}
}
