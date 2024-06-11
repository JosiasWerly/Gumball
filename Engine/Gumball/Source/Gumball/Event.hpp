#pragma once
#ifndef _event
#define _event
#include <iostream>
#include <list>

template<class ...TArgs>
class Event {
protected:
	Event() = default;

public:
	virtual void call(TArgs ...args) = 0;
};

template<class ...TArgs>
class EventFunction : public Event <TArgs...> {
	typedef void(*TFnx)(TArgs...);
	TFnx fnx = nullptr;

public:
	EventFunction(TFnx fnx) :
		fnx(fnx) {
	}
	virtual void call(TArgs ...args) override {
		fnx(args...);
	}
};

template<class TObj, class ...TArgs>
class EventMethod : public Event <TArgs...> {
	typedef void(TObj:: *TMethod)(TArgs...);
	TObj *obj = nullptr;
	TMethod method = nullptr;

public:
	EventMethod(TObj *obj, TMethod method) :
		obj(obj),
		method(method) {
	}
	virtual void call(TArgs ...args) override {
		(obj->*method)(args...);
	}
};

template<class ...TArgs>
class Dispatcher {
protected:
	typedef Event<TArgs...> TEvent;

	std::list<TEvent *> events;

public:
	~Dispatcher() {
		for (auto e : events)
			delete e;
	}

	void bindFunction(void (*function)(TArgs...)) {
		events.push_back(new EventFunction<TArgs...>(function));
	}
	template<class TObj> void bindMethod(TObj *obj, void (TObj::*method)(TArgs...)) {
		events.push_back(new EventMethod<TObj, TArgs...>(obj, method));
	}

	void broadcast(TArgs ...args) {
		for (auto e : events) {
			e->call(args...);
		}
	}
};


#endif // !_event