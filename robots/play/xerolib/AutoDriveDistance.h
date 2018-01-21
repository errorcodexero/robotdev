#pragma once

#include "AutonomousStepBase.h"
#include <memory>

namespace xerolib
{
	class DriveBase;

	class AutoDriveDistance : public AutonomousStepBase
	{
	public:
		AutoDriveDistance(AutonomousControllerBase &controller, std::shared_ptr<DriveBase> drive_p, double distance);
		~AutoDriveDistance();

		virtual void start();
		virtual bool run();
		virtual void end();
		virtual std::string toString();

	private:
		std::shared_ptr<DriveBase> m_drive_base_p;
		double m_distance;
	};
}
