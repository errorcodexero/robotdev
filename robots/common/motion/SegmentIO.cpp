#include "SegmentIO.h"
#include "StringUtils.h"
#include "Segment.h"
#include <fstream>
#include <iostream>

namespace xero
{
	namespace motion
	{
		bool SegmentIO::writeSegments(const std::string &filename, const std::vector<Segment> &segments)
		{
			std::ofstream out(filename);

			if (out.bad() || out.fail())
			{
				std::cerr << "writeSegments: cannot open file '" << filename << "' for writing" << std::endl;
				return false;
			}
			
			out << "t";
			out << ",path_x";
			out << ",path_y";
			out << ",path_theta";
			out << ",path_pos";
			out << ",path_vel";
			out << ",path_accel";
			out << ",path_jerk";
			out << std::endl;

			double current = 0.0;
			for (const Segment &seg : segments)
			{
				out << current;
				current += seg.getDT();
				out << "," << seg.getX();
				out << "," << seg.getY();
				out << "," << seg.getHeading();
				out << "," << seg.getPosition();
				out << "," << seg.getVelocity();
				out << "," << seg.getAcceleration();
				out << "," << seg.getJerk();
				out << std::endl;
			}

			return true;
		}

		bool SegmentIO::readSegments(const std::string &filename, std::vector<Segment> &segments)
		{
			std::ifstream in(filename);
			if (in.bad() || in.fail())
			{
				std::cerr << "readSegments: cannot open file '" << filename << "' for reading" << std::endl;
				return false;
			}

			std::string line;
			bool first = true;

			while (std::getline(in, line))
			{
				std::vector<std::string> words = xero::motion::StringUtils::split(line);
				if (words.size() == 0)
					continue;

				if (words.size() != 8)
				{
					std::cerr << "readSegments: illegal line in file, expected eight common seperated values" << std::endl;
					std::cerr << "line: '" << line << "'" << std::endl;
					segments.clear();
					return false;
				}

				if (first)
				{
					//
					// The first line has the names of the columns
					//
					first = false;
					continue;
				}

				std::vector<double> values = xero::motion::StringUtils::parseDouble(words);
				if (values.size() == 0)
				{
					segments.clear();
					return false;
				}

				Segment seg(values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7]);
				segments.push_back(seg);
			}

			return true;
		}
	}
}
