#include "ChainMachine.hpp"

using namespace Flow;

void ChainMachine::run() {
	if (!chain.size())
		return;

	chain[ctrl.current](ctrl);
	if (ctrl.move) {
		ctrl.last = ctrl.current;
		ctrl.current = (++ctrl.current) % chain.size();
	}
}
ChainMachine &ChainMachine::operator<<(Chain ls) {
	chain.insert(chain.begin(), ls.begin(), ls.end());
	return *this;
}