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
		StateMachine();
		void run();
		void set(THash key);
		State &push(THash key);
		State &operator[](THash key) { return states.at(key); }
		const State &operator[](THash key) const { return states.at(key); }
		const State &getActiveState() { return this->operator[](curKey); }
	};
};
#endif // !__statemachine