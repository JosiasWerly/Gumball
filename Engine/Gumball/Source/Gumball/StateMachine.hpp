#pragma once
#ifndef __statemachine
#define __statemachine

#include <unordered_map>

//template<class TKey, class TState>
class StateMachine {
	typedef int TKey;
	typedef float TState;

	std::unordered_map<TKey, TState> states;
	TKey key;
public:


	const TState &getState(const TKey& key) const { return states.at(key); }
	
	const TState &getActiveState() { return getState(key); }
	const TKey &getActiveKey() { return key; }
};

#endif // !__statemachine
