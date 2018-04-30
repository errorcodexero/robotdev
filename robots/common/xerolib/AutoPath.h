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
			: AutonomousStepBase(controller)
		{
			m_db_p = db_p;
			m_maxaccel = maxaccel;
			m_path_p = c.buildPath(maxaccel);
			m_start_pose = c.getStartPose();
			m_reversed = c.isReversed();
		}

		AutoPath(AutonomousControllerBase &controller, std::shared_ptr<DriveBase> db_p, std::shared_ptr<xero::pathfinder::Path> path_p, 
			const xero::math::PositionCS &start, bool reversed = false)
			: AutonomousStepBase(controller)
		{
			m_path_p = path_p;
			m_start_pose = start;
			m_reversed = reversed;
		}

		virtual ~AutoPath()
		{
		}

		virtual void start()
		{
			MessageLogger &log = getLogger();

			xero::pathfinder::RobotState &state = xero::pathfinder::RobotState::get();
			state.reset(m_start_pose);

			log.startMessage(MessageLogger::MessageType::Debug, MODULE_PATHFINDER);
			log << "Path: " << m_path_p->toString();
			log.endMessage();

			m_db_p->followPath(m_path_p, m_reversed);
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

			result = "follow path, name = '" + m_pathname + "'";
			return result;
		}

	private:
		double m_maxaccel;
		std::shared_ptr<DriveBase> m_db_p;
		std::string m_pathname;
		xero::math::PositionCS m_start_pose;
		std::shared_ptr<xero::pathfinder::Path> m_path_p;
		bool m_reversed;
	};
}

