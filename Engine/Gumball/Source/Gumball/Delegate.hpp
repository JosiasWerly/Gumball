#pragma once
#ifndef __delegate
#define __delegate
#include <list>

template<class ...TArgs>
class IRelay {
protected:
	IRelay() = default;

public:
	virtual void invoke(TArgs ...args) = 0;
};

template <class ... TArgs>
class RelayFunction : public IRelay<TArgs...> {
	using TFunction = void(*)(TArgs...);
	TFunction fn;

public:
	RelayFunction(TFunction fn) : fn(fn) {}
	void invoke(TArgs ...args) { fn(args...); }
};

template <class  TObj, class ... TArgs>
class RelayMethod : public IRelay<TArgs...> {
	using TMethod = void(TObj:: *)(TArgs...);
	TObj *obj = nullptr;
	TMethod method = nullptr;

public:
	RelayMethod(TObj *obj, TMethod method) : obj(obj), method(method) {}
	void invoke(TArgs... args) { (obj->*method)(args...); }
};

template <typename TSignature> class Delegate;

template <class ... TArgs>
class Delegate<void(TArgs...)> {
	using TRelay = IRelay<TArgs...>;
	using TSignature = void(TArgs...);
	std::list<TRelay *> relays;

public:
	Delegate() {
	}
	~Delegate() {
		for (auto &r : relays)
			delete r;
		relays.clear();
	}

	void bind(void (*fn)(TArgs...)) {
		relays.push_back(new RelayFunction(fn));
	}
	template<class TObj> void bind(TObj *obj, void (TObj:: *method)(TArgs...)) {
		relays.push_back(new RelayMethod<TObj, TArgs...>(obj, method));
	}
	void invoke(TArgs... args) {
		for (auto &r : relays)
			r->invoke(args...);
	}
};
#endif // !__delegate