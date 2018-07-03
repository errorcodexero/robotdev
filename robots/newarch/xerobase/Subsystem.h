#pragma once

#include <memory>

namespace xero {
	namespace base {
		
		//
		// Forward declaraion of the robot class.
		//
		class Robot ;
		
		/// \brief This is the base class for any subsystem in the system.
		class Subsystem {

		public:
			/// \brief This is the base class that defines the interface for a directive.
			/// All subsystem directives must be derived from this class
			class Directive {
			public:
				virtual void start()=0;
				virtual void run()=0;
				virtual bool isDone() const = 0;
				virtual void end()=0;
			} ;

		public:
			/// \brief create a new subsystem
			Subsystem(Robot &robot) ;

			/// \brief compute the current state of the robot.
			/// The subsystem generally reads and associated input sensors and
			/// computes a state that is meaningful to users of the subsystem.

     		virtual void computeState() = 0 ;

			/// \brief set the current directive for the subsystem
			/// \param directive the new directive for the subsystem
			/// \return true if the directive is accepted, false if not
			virtual bool setDirective(std::shared_ptr<Directive> directive);

			/// \brief return a constant pointer to the current directive
			/// \returns  a constant pointer to the current directive
			const std::shared_ptr<Directive> getDirective() const;

			/// \brief returns a pointer to the current directive
			/// \returns a pointer to the current directive
			std::shared_ptr<Directive> getDirective() ;

			/// \brief set output actuators associated with the subsystem
			/// The output actuators are set to achieve the currently active
			/// directive based on the current state of the subsystem.
			virtual void run() = 0 ;

			/// \brief returns true if the subsystem is done with the current directive
			/// \returns true if the subsystem is done with the current directive
			virtual bool isDone() const;

		private:
			//
			// A reference to the robot object that contains this subsystem
			//
			Robot& robot_;

			//
			// The currently active directive
			//
			std::shared_ptr<Directive> directive_;
		} ;
	}
}
