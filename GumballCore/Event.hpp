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


// ######ByChecking######
//if (InputSystem::isKeyDown(Input::EKeyCode::W))
//	v.transform.position.y += 0.01;
//else if (InputSystem::isKeyDown(Input::EKeyCode::S))
//	v.transform.position.y -= 0.01;
//
//if (InputSystem::onKeyReleased(Input::EKeyCode::Q))
//	v.transform.rotator.rotate(0.f, 5.f, 0.f);
//else if (InputSystem::onKeyPressed(Input::EKeyCode::E))
//	v.transform.rotator.rotate(0.f, -5.f, 0.f);
//
// 
// ######ByPooling######
//Input::Event e;
//while (InputSystem::eventPool >> e) {
//	if (e.eventType == Input::EventType::keyboard) {
//		if (e.actionType == Input::EActionType::pressed) {
//			switch (e.keycode) {
//			case Input::EKeyCode::A:
//				v.transform.position.x -= 0.01f;
//				break;
//			case Input::EKeyCode::D:
//				v.transform.position.x += 0.01f;
//				break;
//			case Input::EKeyCode::W:
//				v.transform.position.y += 0.01f;
//				break;
//			case Input::EKeyCode::S:
//				v.transform.position.y -= 0.01f;
//				break;
//			}
//		}
//	}
//}