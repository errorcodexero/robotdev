#pragma once

#include "Subsystem.h"

namespace xero {
	namespace phoenix {
		class Drivebase : public Subsytem {
			class DriveStraightDirective : public Directive {
				double target_distance_;
				
			public:
				DriveStraightDirective(double target_distance);
			}

			double dist_l_, dist_r_;
			double angle_;

		public:
			double getDistL() const;
			double getDistR() const;
			double getAngle() const;

			Drivebase(Robot& robot);
		}
	}
}
