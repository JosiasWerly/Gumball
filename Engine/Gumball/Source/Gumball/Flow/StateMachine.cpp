#include "StateMachine.hpp"

using namespace FSM;
StateMachine::StateMachine() :
	curKey(0) {
	ctrl.event = Controller::Event::set;
}
void StateMachine::run() {
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
void StateMachine::set(THash key) {
	ctrl.event = Controller::Event::set;
	curKey = key;
}
State &StateMachine::push(THash key) {
	states.emplace(key, State());
	return states[key];
}


/*
struct A {
	void foo(int a) { cout << "A:" << a << endl; }
};
void test() {
	using namespace FSM;
	enum class eState {
		Root, Sum, Sub,
		Calculate
	};
	StateMachine sm;
	sm.push(eState::Root);
	sm.push(eState::Sum);
	sm.push(eState::Sub);
	sm.push(eState::Calculate);
	int valA, valB;

	sm[eState::Root].onEnter += [&](Controller &c) {
		cout << "ROOT:\n0. sub\n1. sum" << endl;
		int id = 0;
		cin >> id;
		cout << endl;
		c = id ? eState::Sum : eState::Sub;
	};
	sm[eState::Sum].onEnterFrom[eState::Root] += [&](Controller &c) {
		cout << "SUM: from ROOT" << endl;
		cout << "enter valA and valB" << endl;
		cin >> valA;
		cin >> valB;
		cout << endl;
		c = eState::Calculate;
	};
	sm[eState::Sub].onEnterFrom[eState::Root] += [&](Controller &c) {
		cout << "SUB: from ROOT" << endl;
		cout << "enter valA and valB" << endl;
		cin >> valA;
		cin >> valB;
		cout << endl;
		c = eState::Calculate;
	};
	{
		auto &s = sm[eState::Calculate];
		s.onEnter += [&](Controller &c) {
			c = eState::Root;
		};
		s.onEnterFrom[eState::Sum] += [&](Controller &c) {
			cout << "CALCULATE: from SUM" << endl;
			cout << "res:" << valA + valB << endl;
			cout << endl;
		};
		s.onEnterFrom[eState::Sub] += [&](Controller &c) {
			cout << "CALCULATE: from SUB" << endl;
			cout << "res:" << valA - valB << endl;
			cout << endl;
		};
	}


	A a;
	Event<void(int a)> aa;
	aa += [](int a) { cout << "fnx" << a << endl; };
	aa += [&](int a) { cout << "ref" << a << endl; };
	aa += [=](int a) { cout << "cpy" << a << endl; };
	aa.operator+=<A>({ &a, &A::foo });
	aa(2);
	while (1) { sm.run(); }
}
*/