#pragma once
#ifndef _event
#define _event
#include <iostream>
#include <list>
using namespace std;


template<class ...TArgs>
class IForwardBind {
protected:
	IForwardBind(){}
public:
	virtual void call(TArgs ...args) {}
};

template<class ...TArgs>
class ForwardFunction : 
	public IForwardBind <TArgs...> {

	typedef void(*TFnx)(TArgs...);
	TFnx fnx = nullptr;
public:
	ForwardFunction(TFnx fnx) :
		fnx(fnx) {
	}
	virtual void call(TArgs ...args) override {
		fnx(args...);
	}
};

template<class TObj, class ...TArgs>
class ForwardMethod :
	public IForwardBind <TArgs...> {

	typedef void(TObj::*TMethod)(TArgs...);

	TObj* obj = nullptr;
	TMethod method = nullptr;
public:
	ForwardMethod(TObj *obj, TMethod method) :
		obj(obj),
		method(method) {
	}
	virtual void call(TArgs ...args) override {
		(obj->*method)(args...);
	}
};

template<class ...TArgs>
class ForwardDispatcher {
protected:
	typedef IForwardBind<TArgs...> TForward;
	list<TForward*> boundProcs;
public:
	~ForwardDispatcher() {
	}
	void unbind(TForward* newEvent) {
		boundProcs.remove(newEvent);
	}
	void bind(TForward* newEvent) {
		boundProcs.push_back(newEvent);
	}
	
	void broadcast(TArgs ...args) {
		for (auto& e : boundProcs)
			e->call(args...);
	}
};

template<class TEvent>
class EventPool {
	list<TEvent> events;
public:
	EventPool& operator<<(TEvent data) {
		events.push_back(data);
		return *this;
	}
	bool operator>>(TEvent &data) {
		if (hasEvents()) {
			data = events.back();
			events.pop_back();
			return true;
		}
		return false;
	}
	void clearPool() {
		events.clear();
	}
	Inline bool hasEvents() { return events.size(); }
};

#endif // !_event
