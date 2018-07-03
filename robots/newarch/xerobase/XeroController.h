#pragma once

namespace xero {
	namespace base {
		class Robot ;
		
		class XeroController {
		public:
			XeroController(Robot &robot) ;
			virtual void run() = 0 ;

		private:
			Robot &robot_ ;
		} ;
	}
}
