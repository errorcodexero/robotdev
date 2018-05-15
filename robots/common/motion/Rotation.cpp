#include "Rotation.h"
#include "Position.h"

namespace xero
{
	namespace motion
	{
		Rotation::Rotation(const Position &pos)
		{
			m_cos = pos.getX();
			m_sin = pos.getY();
			normalize();
		}
	}
}
