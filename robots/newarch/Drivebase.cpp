#include "Drivebase.h"

namespace xero {
	namespace phoenix {
		void Drivebase::NullDirective::start() {}
		void Drivebase::NullDirective::run() {}
		bool Drivebase::NullDirective::isDone() {
			return true;
		}
		void Drivebase::NullDirective::end() {}

		void Drivebase::DriveStraightDirective::start() {}
		void Drivebase::DriveStraightDirective::run() {

		}
		bool Drivebase::DriveStraightDirective::isDone() {
			return true;
		}
		void Drivebase::DriveStraightDirective::end() {}
		Drivebase::DriveStraightDirective::DriveStraightDirective(double target_distance) {
			target_distance_ = target_distance;
		}

		bool Drivebase::setDirective(Directive& directive) {
			if (!directive_.isDone())
				return false;

			directive_.end();
			directive_ = directive;
			directive_.start();			

			return true;
		}

		const Directive& getDirective() const {
			return directive_;
		}

		bool isDone() const {
			return directive_.isDone();
		}

		Drivebase::Drivebase() {
			directive_ = NullDirective();
		}
	}
}
