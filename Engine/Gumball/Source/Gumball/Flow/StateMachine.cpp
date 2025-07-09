#include "StateMachine.hpp"

using namespace Flow;
StateMachine::StateMachine() {
}
void StateMachine::run() {
	switch (ctrl.event) {
		case Controller::Event::update:
			current->pin().onUpdate(ctrl);
			return;
		case Controller::Event::set:
			ctrl.event = Controller::Event::update;
			current->pin().onEnter(ctrl);
			break;
		case Controller::Event::move:
			ctrl.event = Controller::Event::update;
			{
				State &st = current->pin();
				auto to = st.onExitTo.find(ctrl.next);
				if (to != st.onExitTo.end())
					to->second(ctrl);
				current->pin().onExit(ctrl);
			}
			ctrl.last = ctrl.current;
			ctrl.current = ctrl.next;
			ctrl.next = 0;
			current = &root[ctrl.current];
			{
				State &st = current->pin();
				auto to = st.onEnterFrom.find(ctrl.next);
				if (to != st.onEnterFrom.end())
					to->second(ctrl);
				current->pin().onEnter(ctrl);
			}
			break;
	}
}
void StateMachine::set(THash key) {
	ctrl.event = Controller::Event::set;
	current = &root[key];
}