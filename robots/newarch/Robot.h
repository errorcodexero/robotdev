#pragma once

#include "Subsystem.h"

typedef std::shared_ptr<Subsystem> SubsystemPtr;
typedef std::list<SubsystemPtr> SubsystemList;

namespace xero {
	namespace phoenix {
		class Robot : public frc::SampleRobot {
			const target_loop_time_;

			void robotLoop();

			std::shared_ptr<Controller> controller_;
			
			SubsystemList subsystems_;
		}	
	}
}
