#pragma once
#ifndef __statemachine
#define __statemachine


#include <unordered_map>
#include "Dispatcher.hpp"

namespace FSM {//NOTE: change to Flow
	struct THash {
		int val = 0;

		THash() = default;
		THash(const THash &other) : val(other.val) {}
		THash(THash &&other) noexcept : val(other.val) {}
		template<class T> THash(T value) : val(int(value)) {}
		template<class T> operator T() { return T(val); }
		template<class T> operator const T() const { return T(val); }

		bool operator==(const THash &other) const { return val == other.val; }
	};
	struct THashOperations {
		std::size_t operator()(const THash &key) const { return std::hash<int>()(key.val); }
		bool operator()(const THash &a, const THash &b) const { return a.val == b.val; }
	};

	class Controller {
		friend class StateMachine;

		enum class Event : char { none, set, move };
		Event event = Event::move;
		int last, next;

	public:
		Controller() : last(0), next(0) {}
		void operator=(THash next) { this->next = next; event = Event::move; }
		void to(THash next) { this->next = next; event = Event::move; }
		THash to() const { return next; }
		THash from() const { return last; }
	};

	struct State {
		typedef Event<void(Controller &ctrl)> EObserver;
		typedef Event<void(const Controller &ctrl)> ConstEObserver;
		using UMap = std::unordered_map<THash, EObserver, THashOperations, THashOperations>;
		
		EObserver onEnter;
		UMap onEnterFrom;
		
		ConstEObserver onExit;
		UMap onExitTo;
		State() = default;
	};

	class StateMachine {
		int curKey;
		Controller ctrl;
		std::unordered_map<int, State> states;
	
	public:
		StateMachine() : 
			curKey(0) {
			ctrl.event = Controller::Event::set;
		}
		void run() {
			switch (ctrl.event) {
				case Controller::Event::none: return;
				case Controller::Event::set:
					ctrl.event = Controller::Event::none;
					states.at(curKey).onEnter(ctrl);
					break;
				case Controller::Event::move:
					ctrl.event = Controller::Event::none;
					const int oldKey = curKey;
					{
						State &oldState = states.at(curKey);
						auto exitTo = oldState.onExitTo.find(ctrl.next);
						if (exitTo != oldState.onExitTo.end())
							exitTo->second(ctrl);
						oldState.onExit(ctrl);
					}
					ctrl.last = curKey;
					curKey = ctrl.next;
					{
						State &newState = states.at(curKey);
						auto enterFrom = newState.onEnterFrom.find(oldKey);
						if (enterFrom != newState.onEnterFrom.end())
							enterFrom->second(ctrl);
						newState.onEnter(ctrl);
					}
					break;
			}
		}
		void set(THash key) {
			ctrl.event = Controller::Event::set;
			curKey = key;
		}
		State &push(THash key) {
			states.emplace(key, State());
			return states[key];
		}
		State &operator[](THash key) {
			return states.at(key);
		}
		const State &getState(const THash &key) const { return states.at(key); }
		const State &getActiveState() { return getState(curKey); }
	};
};

#endif // !__statemachine