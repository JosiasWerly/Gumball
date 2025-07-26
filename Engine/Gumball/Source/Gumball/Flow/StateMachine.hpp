#pragma once
#ifndef __statemachine
#define __statemachine

#include "Common.hpp"
#include "Dispatcher.hpp"

#include <unordered_map>

namespace Flow {
	class StateMachine {
	public:
		class Controller {
			friend class StateMachine;

			enum class eSignal : char { update, set, move };
			eStatus status = eStatus::nominal;
			eSignal event = eSignal::move;
			int last, current, next;

		public:
			Controller() : last(0), current(0), next(0) {}
			void operator=(THash next) { this->next = next; event = eSignal::move; }
			void to(THash next) { this->next = next; event = eSignal::move; }
			THash to() const { return next; }
			THash now() const { return current; }
			THash from() const { return last; }
		};
		struct Node {
			using Delegate = TDelegate<void(Controller &ctrl)>;
			using ConstDelegate = TDelegate<void(const Controller &ctrl)>;
			using Delegates = std::unordered_map<THash, Delegate, THashOperations, THashOperations>;
			using ConstDelegates = std::unordered_map<THash, ConstDelegate, THashOperations, THashOperations>;
		
			Delegate onEnter;
			Delegate onUpdate;
			ConstDelegate onExit;
			ConstDelegates onExitTo;
			Node() = default;
		};
	
		StateMachine() = default;
		void run();
		void set(THash key);
		Node &operator[](THash key) { return nodes[key]; }
		const Node &operator[](THash key) const { return nodes.at(key); }
		const Node &activeNode() const { return *current; }
		eStatus status() const { return ctrl.status; }
	
	private:
		Controller ctrl;
		std::unordered_map<THash, Node, THashOperations> nodes;
		Node* current = nullptr;
	};
};
#endif // !__statemachine