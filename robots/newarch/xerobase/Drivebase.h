#pragma once

#include "Subsystem.h"

namespace xero {
	namespace base {
		
		/// \brief the drivebase for the robot
		class Drivebase : public Subsystem {

			/// \brief This is directive meant for the drivebase to drive straight a given distancex
			class DriveStraightDirective : public Subsystem::Directive {
			public:
				DriveStraightDirective(double target_distance);

				virtual void start() ;
				virtual void run() ;
				virtual bool isDone() const ;
				virtual void end() ;

			private:
				double target_distance_;
			} ;

		public:
			Drivebase(Robot& robot);
			
			double getDistL() const {
				return dist_l_ ;
			}
			
			double getDistR() const {
				return dist_r_ ;
			}
			
			double getAngle() const {
				return angle_ ;
			}

			virtual bool setDirective(std::shared_ptr<Subsystem::Directive> directive) ;


		private:
			double dist_l_, dist_r_;
			double angle_;
		} ;
	}
}
