#pragma once

#include "DriveBase.h"
#include "RobotState.h"
#include "AutonomousStepBase.h"
#include "PathContainer.h"
#include "ModuleDefintions.h"

namespace xerolib
{
	class AutoPath : public AutonomousStepBase
	{
	public:
		AutoPath(AutonomousControllerBase &controller, std::shared_ptr<DriveBase> db_p, xero::pathfinder::PathContainer &c, double maxaccel) 
			: AutonomousStepBase(controller), m_container(c)
		{
			m_db_p = db_p;
			m_maxaccel = maxaccel;
		}

		virtual ~AutoPath()
		{
		}

		virtual void start()
		{
			MessageLogger &log = getLogger();

			xero::pathfinder::RobotState &state = xero::pathfinder::RobotState::get();
			state.reset(m_container.getStartPose());
			auto path_p = m_container.buildPath(m_maxaccel);

			log.startMessage(MessageLogger::MessageType::Debug, MODULE_PATHFINDER);
			log << "Path: " << path_p->toString();
			log.endMessage();

			m_db_p->followPath(path_p, m_container.isReversed());
		}

		virtual bool run()
		{
			//
			// Do nothing, waiting on the drivebase to finish the path
			//
			return m_db_p->isPathFinished();
		}

		virtual void end()
		{
			m_db_p->stop();
		}

		virtual std::string toString()
		{
			std::string result;

			result = "follow path, name = '" + m_container.getName() + "'";
			return result;
		}

	private:
		double m_maxaccel;
		std::shared_ptr<DriveBase> m_db_p;
		xero::pathfinder::PathContainer &m_container;
	};
}

