#pragma once

#include "Position.h"
#include "Rotation.h"
#include "PositionAngle.h"

namespace xero
{
	namespace math
	{
		class PositionCS
		{
		private:
			static constexpr double kDelta = 1e-9;

		private:
			//
			// The position of the robot in 2D space
			//
			Position m_position;

			//
			// The rotation of the robot, stored as the sine and cosine
			// of the angle
			//
			Rotation m_rotation;

		public:
			PositionCS()
			{
			}

			PositionCS(const Position &p, const Rotation &r)
			{
				m_position = p;
				m_rotation = r;
			}

			virtual ~PositionCS()
			{
			}

			const Position &getPos() const
			{
				return m_position;
			}

			double getX() const
			{
				return m_position.getX();
			}

			double getY() const
			{
				return m_position.getY();
			}

			void setPosition(const Position &pos)
			{
				m_position = pos;
			}

			const Rotation &getRotation() const
			{
				return m_rotation;
			}

			void setRotation(const Rotation &r)
			{
				m_rotation = r;
			}

			static PositionCS fromTranslation(const Position &p)
			{
				Rotation r;
				return PositionCS(p, r);
			}

			PositionCS transformBy(const PositionCS &cs) const
			{
				Position p = m_position.translateBy(cs.getPos().rotateBy(m_rotation));
				Rotation r = m_rotation.rotateBy(cs.getRotation());
				return PositionCS(p, r);
			}

			PositionCS inverse() const
			{
				Rotation r = m_rotation.inverse();
				Position p = m_position.inverse().rotateBy(r);
				return PositionCS(p, r);
			}

			PositionCS normal() const
			{
				return PositionCS(m_position, m_rotation.normal());
			}

			Position intersection(const PositionCS &pcs) const
			{
				if (m_rotation.isParallel(pcs.getRotation()))
					return Position(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());

				Position p;
				if (std::fabs(m_rotation.getCos()) < std::fabs(pcs.getRotation().getCos()))
				{
					p = intersection(*this, pcs);
				}
				else
				{
					p = intersection(pcs, *this);
				}

				return p;
			}


			bool isColinear(const PositionCS &pcs) const
			{
				PositionAngle pa = log(inverse().transformBy(pcs));
				return std::fabs(pa.getY()) < kDelta && std::fabs(pa.getAngle()) < kDelta;
			}

			PositionCS interpolate(const PositionCS &pcs, double x) const
			{
				PositionCS p;

				if (x <= 0)
				{
					p = *this;
				}
				else if (x >= 1)
				{
					p = pcs;
				}
				else
				{
					PositionCS t = inverse();
					PositionCS t2 = t.transformBy(pcs);
					PositionAngle t3 = PositionCS::log(t2);
					PositionAngle pa = PositionCS::log(inverse().transformBy(pcs));
					p = transformBy(PositionCS::exp(pa.scaled(x)));
				}

				return p;
			}

			static PositionCS fromRotation(const Rotation &r)
			{
				Position p;
				return PositionCS(p, r);
			}

			static PositionCS exp(const PositionAngle &pa)
			{
				double sinth = std::sin(pa.getAngle());
				double costh = std::cos(pa.getAngle());
				double nsin, ncos;

				if (std::fabs(pa.getAngle()) < kDelta)
				{
					nsin = 1.0 - 1.0 / 6.0 * pa.getAngle() * pa.getAngle();
					ncos = 0.5 * pa.getAngle();
				}
				else
				{
					nsin = sinth / pa.getAngle();
					ncos = (1.0 - costh) / pa.getAngle();
				}

				Position p(pa.getX() * nsin - pa.getY() * ncos, pa.getX() * ncos + pa.getY() * nsin);
				Rotation r(costh, sinth, false);
				return PositionCS(p, r);
			}

			static PositionAngle log(const PositionCS &pcs)
			{
				double th = pcs.getRotation().getRadians();
				double cm1 = pcs.getRotation().getCos() - 1.0;
				double tmp;

				if (std::fabs(cm1) < kDelta)
				{
					tmp = 1.0 - 1.0 / 12.0 * th * th;
				}
				else
				{
					tmp = -(th / 2.0 * pcs.getRotation().getSin()) / cm1;
				}

				Rotation r(tmp, -th / 2.0, false);
				Position p = pcs.getPos().rotateBy(r);
				return PositionAngle(p.getX(), p.getY(), th);
			}


		protected:
			static Position intersection(const PositionCS &first, const PositionCS &second)
			{
				Position pt(first.getRotation().getCos(), first.getRotation().getSin());
				double t2 = second.getRotation().getTan();
				double num = (first.getPos().getX() - second.getPos().getX()) * t2 + second.getPos().getY() - first.getPos().getY();
				double den = first.getRotation().getSin() - first.getRotation().getCos() * t2;
				return first.getPos().translateBy(pt.scale(num / den));
			}
		};
	}
}