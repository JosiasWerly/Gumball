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
			THash from() const { return last; }
			THash now() const { return current; }
			void next() { move = true; }
			void finish(bool success) { status = success ? eStatus::success : eStatus::fail; }
		};
		using Delegate = Signal<void(Controller &)>;
		using Chain = std::vector<Delegate>;
	
		ChainMachine() = default;
		void run();
	
		ChainMachine &operator<<(Chain ls);
		Delegate &operator[](THash key) { return chain[key]; }
		const Delegate &operator[](THash key) const { return chain.at(key); }
		eStatus status() const { return ctrl.status; }
	
	private:
		eStatus sts;
		Controller ctrl;
		Chain chain;
	};
};
#endif // !__chainmachine