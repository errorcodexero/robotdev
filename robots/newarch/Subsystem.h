#pragma once

namespace xero {
	namespace base {
		class Subsystem {
			class Directive {
			public:
				virtual void start()=0;
				virtual void run()=0;
				virtual bool isDone()=0 const;
				virtual void end()=0;
			}

			class NullDirective : public Directive {
				
			}

			Robot& robot_;
			std::shared_ptr<Directive> directive_;

		public:
			void computeState();

			bool setDirective(std::shared_ptr<Directive> directive);
			const std::shared_ptr<Directive> getDirective() const;

			void run();
			bool isDone() const;			
		}
	}
}
