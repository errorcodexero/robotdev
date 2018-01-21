#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
#include <list>
#include <limits>
#include "DataDestination.h"

namespace xerolib
{
	//
	// Ok here is how this works.  The normal robot mode, a new instance of the
	// robot object is created that contains this data logger.  The robot creates columns
	// during initialization,and populates those columns with data during each robot loop.
	//
	// At the end of each row, the data is sent for that row to any registered data destinations.
	//
	//
	class DataLogger
	{
	public:
		static const size_t badcol;

	public:
		DataLogger();
		~DataLogger();

		void addDestination(std::shared_ptr<DataDestination> dest_p)
		{
			m_destinations.push_back(dest_p);
		}

		void addDestination(const std::string &filename);

		size_t createColumn(const std::string &name)
		{
			size_t index = m_names.size();
			m_names.push_back(name);
			m_values.resize(m_names.size());
			m_present.resize(m_names.size());
			return index;
		}

		void logData(size_t index, double value)
		{
			//
			// This is a convention to turn off a given value for logging
			//
			if (index == badcol)
				return;

			if (index >= m_values.size())
			{
				std::runtime_error err("DataLogger::putValue() invalid index");
				throw err;
			}
			m_values[index] = value;
			m_present[index] = true;
		}


		void startDataSet()
		{
			for (auto dest_p : m_destinations)
				dest_p->startDataSet(m_names);

			m_first = false;
		}

		void startRow()
		{
			if (m_first)
				startDataSet();

			for (auto dest_p : m_destinations)
				dest_p->startRow();

			std::fill(m_present.begin(), m_present.end(), false);
		}

		void endRow()
		{
			for (auto dest_p : m_destinations)
				dest_p->endRow(m_names, m_values, m_present);
		}

		void endDataSet()
		{
			for (auto dest_p : m_destinations)
				dest_p->endDataSet();
		}

	private:
		std::vector<std::string> m_names;
		std::vector<double> m_values;
		std::vector<bool> m_present;
		std::list<std::shared_ptr<DataDestination>> m_destinations;
		bool m_first;
	};
}

