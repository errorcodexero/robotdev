#include "AutoDriveDistance.h"
#include "DriveBase.h"
#include <sstream>

namespace xerolib
{
	AutoDriveDistance::AutoDriveDistance(AutonomousControllerBase &controller, std::shared_ptr<DriveBase> drive_p, double distance)
		: AutonomousStepBase(controller)
	{
		m_distance = distance;
		m_drive_base_p = drive_p;
	}

	AutoDriveDistance::~AutoDriveDistance()
	{
	}

	void AutoDriveDistance::start()
	{
		m_drive_base_p->driveStraight(m_distance);
	}

	bool AutoDriveDistance::run()
	{
		return m_drive_base_p->isIdle();
	}

	void AutoDriveDistance::end()
	{
	}

	std::string AutoDriveDistance::toString()
	{
		std::stringstream strm;

		strm << "drive distance, distance = " << m_distance << " inches, ";
		return strm.str();
	}
}
