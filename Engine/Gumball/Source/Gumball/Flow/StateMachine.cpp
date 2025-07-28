#include "StateMachine.hpp"

using namespace Flow;
void StateMachine::run() {
	switch (ctrl.event) {
		case Controller::eSignal::set:
			current.first = ctrl.next;
			current.second = &nodes[ctrl.next];
			current.second->onEnter(ctrl);
			ctrl.event = Controller::eSignal::idle;
			break;
		case Controller::eSignal::move:
			{
				auto to = current.second->onExitTo.find(ctrl.next);
				if (to != current.second->onExitTo.end())
					to->second(ctrl);
				current.second->onExit(ctrl);
			}
			ctrl.last = ctrl.current;
			ctrl.current = ctrl.next;
			ctrl.next = 0;
			current.second = &nodes[ctrl.current];
			{
				current.second->onEnter(ctrl);
			}
			ctrl.event = Controller::eSignal::idle;
			break;
	}
}
void StateMachine::tick() {
	if (current.second)
		current.second->onTick(ctrl);
}
void StateMachine::set(THash key) {
	ctrl.event = Controller::eSignal::set;
	ctrl.next = key;
}