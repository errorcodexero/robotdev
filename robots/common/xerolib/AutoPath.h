#pragma once

#include "DriveBase.h"
#include "AutonomousStepBase.h"
#include "PathContainer.h"

namespace xerolib
{
	class AutoPath
	{
	public:
		AutoPath(std::shared_ptr<DriveBase> db_p, xero::pathfinder::PathContainer &c) : m_container(c)
		{
			m_db_p = db_p;
		}

		virtual ~AutoPath()
		{
		}

		virtual void start()
		{
			m_db_p->followPath(m_container.buildPath(), m_container.isReversed());
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
		std::shared_ptr<DriveBase> m_db_p;
		xero::pathfinder::PathContainer &m_container;
	};
}

