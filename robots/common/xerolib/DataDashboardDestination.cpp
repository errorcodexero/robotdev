#include "DataDashboardDestination.h"
#include <wpilib.h>

namespace xerolib
{
    void DataDashboardDestination::endRow(const std::vector<std::string> &names, 
					  const std::vector<double> &data, const std::vector<bool> &present)
    {
	for(size_t i = 0 ; i < names.size() ; i++)
	{
	    if (present[i])
		frc::SmartDashboard::PutNumber(names[i], data[i]) ;
	}
    }
}
