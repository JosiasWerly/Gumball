#pragma once
#ifndef __dispatcher
#define __dispatcher
#include <list>
#include <functional>

template<typename ...TSignature>class TDelegate;
template<typename ...TSignature>class TEvent;

template <class TRet, class ...TArgs>
class TDelegate<TRet(TArgs...)> {
	using TObserver = std::function<TRet(TArgs...)>;

public:
	TDelegate() = default;
	~TDelegate() = default;

	template<class TObj>
	void bind(TObj *obj, void (TObj:: *method)(TArgs...)) {
		target = [=](TArgs... args) { (obj->*method)(args...); };
	}
	void bind(TObserver &&function) {
		target = std::move(function);
	}
	
	Inline void invoke(TArgs... args) { 
		if(target)
			target(args...); 
	}
	Inline void operator()(TArgs... args) { 
		if(target)
			target(args...); 
	}
	bool isBound() const { return target.operator bool(); }

private:
	TObserver target;
};

template <class ...TArgs>
class TEvent<void(TArgs...)> {
public:
	using TObserver = std::function<void(TArgs...)>;

	TEvent() = default;
	~TEvent() = default;
	
	template<class TObj>
	void bind(TObj *obj, void (TObj:: *method)(TArgs...)) {
		targets.emplace_back([=](TArgs... args) { (obj->*method)(args...); });
	}
	void bind(TObserver &&function) {
		targets.push_back(std::move(function));
	}

	Inline void invoke(TArgs... args) { 
		for (auto &t : targets)
			t(args...);
	}
	Inline void operator()(TArgs... args) 
	{ 
		for (auto &t : targets)
			t(args...);
	}
	bool isBound() const { return targets.size() > 0; }

private:
	std::list<TObserver> targets;
};
#endif // !__dispatcher