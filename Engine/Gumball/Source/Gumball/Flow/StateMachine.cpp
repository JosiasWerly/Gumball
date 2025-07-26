#include "StateMachine.hpp"

using namespace Flow;
void StateMachine::run() {
	switch (ctrl.event) {
		case Controller::eSignal::set:
			ctrl.event = Controller::eSignal::update;
			current->onEnter(ctrl);
			break;
		case Controller::eSignal::move:
			ctrl.event = Controller::eSignal::update;
			{
				auto to = current->onExitTo.find(ctrl.next);
				if (to != current->onExitTo.end())
					to->second(ctrl);
				current->onExit(ctrl);
			}
			ctrl.last = ctrl.current;
			ctrl.current = ctrl.next;
			ctrl.next = 0;
			current = &nodes[ctrl.current];
			{
				current->onEnter(ctrl);
			}
			break;
	}
	if (current)
		current->onUpdate(ctrl);
}
void StateMachine::set(THash key) {
	ctrl.event = Controller::eSignal::set;
	current = &nodes[key];
}