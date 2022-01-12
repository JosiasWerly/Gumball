#pragma once
#ifndef _event
#define _event
#include <iostream>
#include <list>
using namespace std;


template<class ...TArgs>
class IEvent {
protected:
	IEvent(){}
public:
	virtual void call(TArgs ...args) {}
};

template<class ...TArgs>
class EventFnx : 
	public IEvent <TArgs...> {

	typedef void(*TFnx)(TArgs...);
	TFnx fnx = nullptr;
public:
	EventFnx(TFnx fnx) :
		fnx(fnx) {

	}
	virtual void call(TArgs ...args) override {
		fnx(args...);
	}
};

template<class TObj, class ...TArgs>
class EventMember :
	public IEvent <TArgs...> {

	typedef void(TObj::*TMethod)(TArgs...);

	TObj* obj = nullptr;
	TMethod method = nullptr;
public:
	EventMember(TObj *obj, TMethod method) :
		obj(obj),
		method(method) {
	}
	virtual void call(TArgs ...args) override {
		(obj->*method)(args...);
	}
};

template<class ...TArgs>
class Dispatcher {
	typedef IEvent<TArgs...> TEvent;
	list<TEvent*> boundEvents;
public:
	void unbind(TEvent* newEvent) {
		boundEvents.remove(newEvent);
	}
	void bind(TEvent* newEvent) {
		boundEvents.push_back(newEvent);
	}
	void dispatch(TArgs ...args) {
		for (auto& e : boundEvents)
			e->call(args...);
	}
};


#endif // !_event
