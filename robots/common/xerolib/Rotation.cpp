#include "Rotation.h"
#include "Position.h"

namespace xero
{
	namespace math
	{
		Rotation::Rotation(const Position &pos)
		{
			m_cos = pos.getX();
			m_sin = pos.getY();
			normalize();
		}
	}
}
