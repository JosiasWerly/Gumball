#pragma once
#ifndef __statemachine
#define __statemachine

#include "Common.hpp"
#include "Dispatcher.hpp"

#include <unordered_map>

namespace Flow {//NOTE: change to Flow

	class StateMachine {
	public:
		class Controller {
			friend class StateMachine;

			enum class Event : char { update, set, move };
			Event event = Event::move;
			int last, current, next;

		public:
			Controller() : last(0), current(0), next(0) {}
			void operator=(THash next) { this->next = next; event = Event::move; }
			void to(THash next) { this->next = next; event = Event::move; }
			THash to() const { return next; }
			THash now() const { return current; }
			THash from() const { return last; }
		};
		struct State {
			using Observer = Event<void(Controller &ctrl)>;
			using ConstObserver = Event<void(const Controller &ctrl)>;
			using Observers = std::unordered_map<THash, Observer, THashOperations, THashOperations>;
			using ConstObservers = std::unordered_map<THash, ConstObserver, THashOperations, THashOperations>;
		
			Observer onEnter;
			Observers onEnterFrom;
			Observer onUpdate;
			ConstObserver onExit;
			ConstObservers onExitTo;
			State() = default;
		};
		using Node = Node<State>;
	
		StateMachine();
		void run();
		void set(THash key);
		State &operator[](THash key) { return root[key].pin(); }
		const State &operator[](THash key) const { return root[key].pin(); }
		const State &activeState() const { return current->pin(); }
	
	private:
		Controller ctrl;
		Node *current;
		Node root;
	};
};
#endif // !__statemachine