#pragma once
#ifndef __signalPool
#define __signalPool

#include "Common.hpp"
#include <deque>

namespace Flow {
	class SignalPool {
		std::deque<TInt> signals;

	public:
		TInt consumeFront();
		TInt consumeBack();
		Inline void pushFront(TInt signal) { signals.push_front(signal); }
		Inline void pushBack(TInt signal) { signals.push_back(signal); }
		Inline TInt front() { return signals.front(); }
		Inline TInt back() { return signals.back(); }
		Inline const TInt front() const { return  signals.front(); }
		Inline const TInt back() const { return signals.back(); }
		Inline bool isEmpty() const { return signals.empty(); }
	};
};
#endif // !__signalmachine