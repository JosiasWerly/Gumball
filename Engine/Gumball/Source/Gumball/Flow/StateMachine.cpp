#include "StateMachine.hpp"

using namespace Flow::StateMachine;

void StateMachine::Tick() {
	switch (event) {
		case Controller::eEvent::set:
			currentState.first = next;
			currentState.second = &states[next];
			currentState.second->OnEnter();
			event = Controller::eEvent::idle;
			break;
		case Controller::eEvent::move:
		{
			auto to = currentState.second->OnExitTo.find(next);
			if (to != currentState.second->OnExitTo.end())
				to->second();
			currentState.second->OnExit();
		}
		last = current;
		current = next;
		next = 0;
		currentState.first = current;
		currentState.second = &states[current];
		{
			currentState.second->OnEnter();
		}
		event = Controller::eEvent::idle;
		break;
	}
	currentState.second->OnTick();
}