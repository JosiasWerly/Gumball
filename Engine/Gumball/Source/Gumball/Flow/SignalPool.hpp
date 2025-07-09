#pragma once
#ifndef __eventpool
#define __eventpool

#include "Common.hpp"
#include "Dispatcher.hpp"

#include <deque>

namespace Flow {
	class EventPool {
	public:
		using Observer = Event<void(void)>;
		using Observers = std::unordered_map<THash, Observer, THashOperations, THashOperations>;

		class Controller {
			std::deque<THash> newSignals;
		
		public:

			void operator+=(THash signal) {
				newSignals.push_back(signal);
			}
		};

		typedef Event<void(void)> Observer;
		using ObserverMap = std::unordered_map<THash, Observer, THashOperations, THashOperations>;

		EventPool() = default;
		void run() {
			while (signals.size()) {
				auto it = signaleers.find(signals.front());
				if (it != signaleers.end()) {
					(*it).second.invoke();
				}
			}
		}
	private:
		ObserverMap signaleers;
		std::deque<THash> signals;
	};
};
#endif // !__eventpool