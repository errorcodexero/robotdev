#pragma once
#include "DataDestination.h"
#include <fstream>
#include <string>

namespace xerolib
{
	class DataFileDestination : public DataDestination
	{
	public:
		DataFileDestination(const std::string &filename);
		virtual ~DataFileDestination();

		virtual void endRow(const std::vector<std::string> &names,
			const std::vector<double> &data, const std::vector<bool> &present);

		virtual void startDataSet(const std::vector<std::string> &names);


		virtual void endDataSet()
		{
		}

		virtual void startRow()
		{
		}

	private:
		std::ofstream m_stream;
	};
}

