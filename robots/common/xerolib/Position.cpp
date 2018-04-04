#include "Position.h"
#include "Rotation.h"

namespace xero
{
	namespace math
	{
		Position Position::rotateBy(const Rotation &r) const
		{
			double rx = m_x * r.getCos() - m_y * r.getSin();
			double ry = m_y * r.getSin() + m_y * r.getCos();
			return Position(m_x * r.getCos() - m_y * r.getSin(), m_x * r.getSin() + m_y * r.getCos());
		}
	}
}
