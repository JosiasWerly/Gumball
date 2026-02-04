#pragma once
#ifndef __statemachine
#define __statemachine

#include "Common.hpp"
#include "Dispatcher.hpp"

#include <unordered_map>

namespace Flow::StateMachine {
	class Controller {
	protected:
		enum class eEvent : char { idle, set, move };
		eEvent event = eEvent::move;
		int last, current, next;

	public:
		Controller() : last(0), current(0), next(0) {}
		void set(TInt next) { this->next = next; event = eEvent::set; };
		void to(TInt next) { this->next = next; event = eEvent::move; }
		TInt to() const { return next; }
		TInt now() const { return current; }
		TInt from() const { return last; }
	};

	struct State {
		using Delegate = Signal<void()>;
		using Delegates = std::unordered_map<TInt, Delegate, TIntOperators, TIntOperators>;

		Delegate onEnter;
		Delegate onTick;
		Delegate onExit;
		Delegates onExitTo;
	};

	class StateMachine : public Controller {
		Controller ctrl;
		Flow::TIntMap<State> states;
		std::pair<TInt, State *> currentState;
	
	public:	
		StateMachine() = default;
		void tick();
		
		State &operator[](TInt key) { return states[key]; }
		const State &operator[](TInt key) const { return states.at(key); }
	};
};
#endif // !__statemachine