#pragma once

namespace xero {
	namespace base {
		class Robot ;
		
		class ControllerBase {
		public:
			ControllerBase(Robot &robot) ;
			virtual void run() = 0 ;

		protected:
			Robot &getRobot() {
				return robot_ ;
			}

		private:
			Robot &robot_ ;
		} ;
	}
}
