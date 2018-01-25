#pragma once

#include "DataDestination.h"
#include <string>
#include <vector>

namespace xerolib
{
    class DataDashboardDestination : public DataDestination
    {
    public:
	DataDashboardDestination()
	{
	}

	virtual ~DataDashboardDestination()
	{
	}

	virtual void endRow(const std::vector<std::string> &names, 
			    const std::vector<double> &data, const std::vector<bool> &present) ;
	virtual void startDataSet(const std::vector<std::string> &names)
	{
	}
	
	virtual void endDataSet()
	{
	}
	
	virtual void startRow()
	{
	}
    };
}
