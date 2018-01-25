#pragma once

#include "AutonomousStepBase.h"
#include <memory>

namespace xerolib
{
	class DriveBase;

	class AutoDriveBaseChar : public AutonomousStepBase
	{
	public:
		AutoDriveBaseChar(AutonomousControllerBase &controller, std::shared_ptr<DriveBase> drive_p, double voltage, double time, bool linear);
		~AutoDriveBaseChar();

		virtual void start();
		virtual bool run();
		virtual void end();
		virtual std::string toString();

	private:
		enum class State
		{
			Waiting,
			Running,
			Measuring,
			Complete,
		};

	private:
		State m_state;
		double m_next_time;
		double m_voltage;
		double m_duration;
		bool m_linear;
		std::shared_ptr<DriveBase> m_drive_base_p;
	};
}
