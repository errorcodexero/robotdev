#pragma once

#include <string>
#include <vector>

namespace xerolib
{
	class DataDestination
	{
	public:
		DataDestination()
		{
		}

		virtual ~DataDestination()
		{
		}

		virtual void endRow(const std::vector<std::string> &names, 
			const std::vector<double> &data, const std::vector<bool> &present) = 0;
		virtual void startDataSet(const std::vector<std::string> &names) = 0;
		virtual void endDataSet() = 0;
		virtual void startRow() = 0;
	};
}
