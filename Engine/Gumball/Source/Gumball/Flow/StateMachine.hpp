#pragma once
#ifndef __statemachine
#define __statemachine

#include <Gumball/Containers/Delegate.hpp>
#include "Common.hpp"
#include <unordered_map>


namespace Flow::StateMachine {
	class Controller {
	protected:
		enum class eEvent : char { idle, set, move };
		eEvent event = eEvent::move;
		int last, current, next;

	public:
		Controller() : last(0), current(0), next(0) {}
		void Set(TInt next) { this->next = next; event = eEvent::set; };
		void To(TInt next) { this->next = next; event = eEvent::move; }
		TInt To() const { return next; }
		TInt Now() const { return current; }
		TInt From() const { return last; }
	};

	struct State {
		using Delegate = Delegate<void()>;
		using Delegates = std::unordered_map<TInt, Delegate, TIntOperators, TIntOperators>;

		Delegate OnEnter;
		Delegate OnTick;
		Delegate OnExit;
		Delegates OnExitTo;
	};

	class StateMachine : public Controller {
		Controller ctrl;
		Flow::TIntMap<State> states;
		std::pair<TInt, State *> currentState;
	
	public:	
		StateMachine() = default;
		void Tick();
		
		State &operator[](TInt key) { return states[key]; }
		const State &operator[](TInt key) const { return states.at(key); }
	};
};

namespace Flow {
using Fsm = StateMachine::StateMachine;

};
#endif // !__statemachine