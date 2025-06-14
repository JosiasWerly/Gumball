#pragma once
#ifndef __dispatcher
#define __dispatcher
#include <list>
#include <functional>

namespace Dispatcher {
	/*template<class ...TArgs>
	class IObserver {
	public:
		using TInvoke = std::function<void(TArgs...)>;
		virtual TInvoke &capture() = 0;
	
	protected:
		IObserver() = default;	
	};

	template <class ... TArgs>
	class Function : public IObserver<TArgs...> {
		using TFunction = IObserver<TArgs...>::TInvoke;
		TFunction fn = nullptr;

	public:
		Function(TFunction fn) : fn(fn) {}
		TFunction &capture() override { return fn; }
	};

	template <class  TObj, class ... TArgs>
	class Method : public IObserver<TArgs...> {
		using TMethod = IObserver<TArgs...>::TInvoke;
		TMethod mt = nullptr;

	public:
		Method(TObj *obj, void(TObj:: *method)(TArgs...))
			: mt([=](TArgs... args) { (obj->*method)(args); }) {
		}
		TMethod &capture() override { return mt; }
	};*/
};

template<typename ...TSignature>class Event;

template <class TRet, class ...TArgs>
class Event<TRet(TArgs...)> {
public:
	using TObserver = std::function<TRet(TArgs...)>;
	std::list<TObserver> observers;
	template<typename TObj>
	struct Binder {
		TObj *obj;
		TRet(TObj:: *method)(TArgs...);
	};

	Event() = default;
	~Event() {
		observers.clear();
	}

	template<typename TObj>
	Inline void operator+=(Binder<TObj> binder) {
		observers.emplace_back([=](TArgs... args) { (binder.obj->*binder.method)(args...); });
	}
	Inline void operator+=(TObserver &&binder) {
		observers.push_back(std::move(binder));
	}
	Inline void operator()(TArgs... args) {
		for (auto &o : observers)
			o(args...);
	}

	template<class TObj>
	void bind(TObj *obj, void (TObj:: *method)(TArgs...)) {
		this->operator+=(Binder<TObj> { obj, method });
	}
	void bind(TObserver &&binder) {
		this->operator+=(binder);
	}
	void invoke(TArgs... args) {
		for (auto &o : observers)
			o(args...);
	}
};
#endif // !__dispatcher