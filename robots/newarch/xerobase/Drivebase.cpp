#include "Drivebase.h"

namespace xero {
	namespace base {

		//////////////////////////////////////////////////////////////////
		//
		// DriveStraightDirective
		//
		//////////////////////////////////////////////////////////////////

		void Drivebase::DriveStraightDirective::start()  {
		}
		
		void Drivebase::DriveStraightDirective::run() {
		}
		
		bool Drivebase::DriveStraightDirective::isDone() const {
			return true;
		}
		
		void Drivebase::DriveStraightDirective::end() {
		}
		
		Drivebase::DriveStraightDirective::DriveStraightDirective(double target_distance) {
			target_distance_ = target_distance;
		}

		//////////////////////////////////////////////////////////////////
		//
		// Drivebase
		//
		//////////////////////////////////////////////////////////////////

		Drivebase::Drivebase(Robot &robot) : Subsystem(robot, "drivebase") {
		}

		/// \brief set a new directive for the drivebase
		bool Drivebase::setDirective(std::shared_ptr<Subsystem::Directive> directive) {
			//
			// The current directive is not done, we return false
			//
			if (!getDirective()->isDone())
				return false ;

			//
			// Call the base class to set the directive
			//
			if (!Subsystem::setDirective(directive))
				return false ;

			//
			// And now start the directive
			//
			getDirective()->start() ;

			return true ;
		}
	}
}
