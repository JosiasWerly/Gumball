#include "SignalPool.hpp"

using namespace Flow;
THash SignalPool::consumeFront() {
	THash out = signals.front();
	signals.pop_front();
	return out;
}
THash SignalPool::consumeBack() {
	THash out = signals.back();
	signals.pop_front();
	return out;
}