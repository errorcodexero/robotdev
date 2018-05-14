#include "SegmentWriter.h"
#include <fstream>
#include <iostream>

namespace xero
{
	namespace motion
	{
		bool SegmentWriter::writeSegments(const std::string &filename, const std::vector<Segment> &segments)
		{
			std::ofstream out(filename);

			if (out.bad() || out.fail())
			{
				std::cerr << "cannot open file '" << filename << "' for writing" << std::endl;
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
	}
}
