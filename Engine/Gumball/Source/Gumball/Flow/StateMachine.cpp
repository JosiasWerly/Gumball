#include "StateMachine.hpp"

using namespace Flow::StateMachine;

void StateMachine::tick() {
	switch (event) {
		case Controller::eEvent::set:
			currentState.first = next;
			currentState.second = &states[next];
			currentState.second->onEnter();
			event = Controller::eEvent::idle;
			break;
		case Controller::eEvent::move:
		{
			auto to = currentState.second->onExitTo.find(next);
			if (to != currentState.second->onExitTo.end())
				to->second();
			currentState.second->onExit();
		}
		last = current;
		current = next;
		next = 0;
		currentState.first = current;
		currentState.second = &states[current];
		{
			currentState.second->onEnter();
		}
		event = Controller::eEvent::idle;
		break;
	}
	currentState.second->onTick();
}