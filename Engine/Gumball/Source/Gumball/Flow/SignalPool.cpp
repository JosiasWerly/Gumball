#include "SignalPool.hpp"

using namespace Flow;
TInt SignalPool::consumeFront() {
	TInt out = signals.front();
	signals.pop_front();
	return out;
}
TInt SignalPool::consumeBack() {
	TInt out = signals.back();
	signals.pop_front();
	return out;
}