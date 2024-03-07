#pragma once
#ifndef _event
#define _event
#include <iostream>
#include <list>

using namespace std;





template<class ...TArgs>
class Delegate {
protected:
	Delegate(){}

public:
	virtual void call(TArgs ...args) {}
};

template<class ...TArgs>
class DelegateFunction : 
	public Delegate <TArgs...> {

	typedef void(*TFnx)(TArgs...);
	TFnx fnx = nullptr;

public:
	DelegateFunction(TFnx fnx) :
		fnx(fnx) {
	}
	virtual void call(TArgs ...args) override {
		fnx(args...);
	}
};

template<class TObj, class ...TArgs>
class DelegateMethod :
	public Delegate <TArgs...> {

	typedef void(TObj::*TMethod)(TArgs...);
	TObj* obj = nullptr;
	TMethod method = nullptr;

public:
	DelegateMethod(TObj *obj, TMethod method) :
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
	typedef Delegate<TArgs...> TDelegate;
	list<TDelegate *> delegates;

public:
	~Dispatcher() {
	}
	void unbind(TDelegate * newEvent) {
		delegates.remove(newEvent);
	}
	void bind(TDelegate * newEvent) {
		delegates.push_back(newEvent);
	}
	void broadcast(TArgs ...args) {
		for (auto& it : delegates)
			it->call(args...);
	}
};

template<class TEvent>
class EventPool {
	list<TEvent> events;
public:
	EventPool &operator<<(TEvent data) {
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



//template<class ...TArgs>
//class Delegate {
//protected:
//	Delegate() {}
//
//public:
//	virtual void call(TArgs ...args) {}
//};
//
//template<class ...TArgs>
//class DelegateFunction :
//	public Delegate <TArgs...> {
//
//	typedef void(*TFnx)(TArgs...);
//	TFnx fnx = nullptr;
//
//public:
//	DelegateFunction(TFnx fnx) :
//		fnx(fnx) {
//	}
//	virtual void call(TArgs ...args) override {
//		fnx(args...);
//	}
//};
//
//template<class TObj, class ...TArgs>
//class DelegateMethod :
//	public Delegate <TArgs...> {
//
//	typedef void(TObj:: *TMethod)(TArgs...);
//	TObj *obj = nullptr;
//	TMethod method = nullptr;
//
//public:
//	DelegateMethod(TObj *obj, TMethod method) :
//		obj(obj),
//		method(method) {
//	}
//	virtual void call(TArgs ...args) override {
//		(obj->*method)(args...);
//	}
//};
//
//template<class ...TArgs>
//class Dispatcher {
//protected:
//	typedef Delegate<TArgs...> TDelegate;
//	list<TDelegate *> delegates;
//
//public:
//	~Dispatcher() {
//	}
//	void unbind(TDelegate *newEvent) {
//		delegates.remove(newEvent);
//	}
//	void bind(TDelegate *newEvent) {
//		delegates.push_back(newEvent);
//	}
//	void broadcast(TArgs ...args) {
//		for (auto &it : delegates)
//			it->call(args...);
//	}
//};
//
//template<class TEvent>
//class EventPool {
//	list<TEvent> events;
//public:
//	EventPool &operator<<(TEvent data) {
//		events.push_back(data);
//		return *this;
//	}
//	bool operator>>(TEvent &data) {
//		if (hasEvents()) {
//			data = events.back();
//			events.pop_back();
//			return true;
//		}
//		return false;
//	}
//	void clearPool() {
//		events.clear();
//	}
//	Inline bool hasEvents() { return events.size(); }
//};
#endif // !_event