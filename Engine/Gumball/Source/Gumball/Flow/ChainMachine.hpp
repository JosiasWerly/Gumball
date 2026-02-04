#pragma once
#ifndef __chainmachine
#define __chainmachine

#include "Common.hpp"
#include "Dispatcher.hpp"

#include <vector>

namespace Flow {
	class ChainMachine {
	public:
		class Controller {
			friend class ChainMachine;
			
			eStatus status = eStatus::nominal;			
			bool move;
			int last, current;
		
		public:
			TInt from() const { return last; }
			TInt now() const { return current; }
			void next() { move = true; }
			void finish(bool success) { status = success ? eStatus::success : eStatus::fail; }
		};
		using Delegate = Signal<void(Controller &)>;
		using Chain = std::vector<Delegate>;
	
		ChainMachine() = default;
		void run();
	
		ChainMachine &operator<<(Chain ls);
		Delegate &operator[](TInt key) { return chain[key]; }
		const Delegate &operator[](TInt key) const { return chain.at(key); }
		eStatus status() const { return ctrl.status; }
	
	private:
		eStatus sts;
		Controller ctrl;
		Chain chain;
	};
};
#endif // !__chainmachine