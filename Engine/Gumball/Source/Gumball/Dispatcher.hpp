#pragma once
#ifndef __dispatcher
#define __dispatcher
#include <list>

namespace Dispatcher {
	template<class ...TArgs>
	class IObserver {
	protected:
		IObserver() = default;

	public:
		virtual void invoke(TArgs ...args) = 0;
	};

	template <class ... TArgs>
	class Function : public IObserver<TArgs...> {
		using TFunction = void(*)(TArgs...);
		TFunction fn;

	public:
		Function(TFunction fn) : fn(fn) {}
		void invoke(TArgs ...args) { fn(args...); }
	};

	template <class  TObj, class ... TArgs>
	class Method : public IObserver<TArgs...> {
		using TMethod = void(TObj:: *)(TArgs...);
		TObj *obj = nullptr;
		TMethod method = nullptr;

	public:
		Method(TObj *obj, TMethod method) : obj(obj), method(method) {}
		void invoke(TArgs... args) { (obj->*method)(args...); }
	};
};

template <typename TSignature> class Event;

template <class ... TArgs>
class Event<void(TArgs...)> {
	using TObserver = Dispatcher::IObserver<TArgs...>;
	using TSignature = void(TArgs...);
	std::list<TObserver *> observers;

public:
	Event() = default;
	~Event() {
		for (auto &r : observers)
			delete r;
		observers.clear();
	}

	void bind(void (*fn)(TArgs...)) {
		observers.push_back(new Dispatcher::Function(fn));
	}
	template<class TObj> void bind(TObj *obj, void (TObj:: *method)(TArgs...)) {
		observers.push_back(new Dispatcher::Method<TObj, TArgs...>(obj, method));
	}
	void invoke(TArgs... args) {
		for (auto &r : observers)
			r->invoke(args...);
	}
};

#endif // !__dispatcher