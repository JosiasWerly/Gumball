#pragma once
#ifndef __signalPool
#define __signalPool

#include "Common.hpp"
#include <deque>

namespace Flow {
	class SignalPool {
		std::deque<THash> signals;

	public:
		THash consumeFront();
		THash consumeBack();
		Inline void pushFront(THash signal) { signals.push_front(signal); }
		Inline void pushBack(THash signal) { signals.push_back(signal); }
		Inline THash front() { return signals.front(); }
		Inline THash back() { return signals.back(); }
		Inline const THash front() const { return  signals.front(); }
		Inline const THash back() const { return signals.back(); }
		Inline bool isEmpty() const { return signals.empty(); }
	};
};
#endif // !__signalmachine