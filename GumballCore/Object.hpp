#pragma once
#ifndef _object
#define _object

#include "Definitions.hpp"
#include "EngineSystem.hpp"

#include <string>
#include <list>
using namespace std;

class Object;
class ObjectPackage;
class MemorySystem;
class IEngineSystem;


enum class EObjectPackage {
	asset,
	game
};

class Object {
	const string name;
protected:
	Object(string name) {}
public:
	void setName(string newName) {
	}
	const string &getName() { 
		return name; 
	}
};

class IObjectPackage {
	list<Object> objects;
public:

};


class MemorySystem :
	public IEngineSystem {
public:
};


//#include <iostream>
//#include <list>
//#include <map>
//#include <unordered_map>
//using namespace std;
//
//#include "EngineSystem.hpp"
//#include "Var.hpp"
//#include "Patterns.hpp"

//typedef long long ObjAddrs;
//
//enum class EObjectLifecycle {
//	beingStarted, nominal, beingDeleted
//};
//
//class Object {
//	friend class ObjectSystem;
//	template<class T> friend Var<T> Instantiate(Object* obj);
//	friend void Destroy(Var<Object> obj);
//
//
//	string name = "";
//	EObjectLifecycle lifecycle = EObjectLifecycle::beingStarted;
//public:
//	virtual ~Object() {}
//
//	virtual void start() {}
//	virtual void end() {}
//	virtual void tick(float deltaTime) {}
//
//	Inline string getName() const { return name; }
//	Inline bool isValid() const { return lifecycle == EObjectLifecycle::nominal; }
//};
//
//class ObjectSystem :
//	public IEngineSystem {
//
//	template<class T> friend Var<T> Instantiate(Object* obj);
//	friend void Destroy(Var<Object> obj);
//
//	list<Var<Object>> objects, starting, deleting;
//public:
//	Var<Object>* findObject(string name) {
//		for (auto& obj : objects) {
//			if (obj->getName() == name)
//				return &obj;
//		}
//		return nullptr;
//	}
//	void tick(float deltaTime) override {
//		if (starting.size()) {
//			for (auto& obj : starting) {
//				obj->start();
//				obj->lifecycle = EObjectLifecycle::nominal;
//			}
//			starting.clear();
//		}
//		if (deleting.size()) {
//			for (auto& obj : deleting) {
//				objects.remove(obj);
//				obj->end();
//			}
//			deleting.clear();
//		}
//		for (auto& obj : objects)
//			obj->tick(deltaTime);
//	}
//};
//
//template<class T>
//Var<T> Instantiate(Object* obj) {
//	Var<Object> newInstance = obj;
//	obj->lifecycle = EObjectLifecycle::beingStarted;
//	ObjectSystem::instance().objects.push_back(newInstance);
//	return newInstance;
//}
//
//static void Destroy(Var<Object> obj) {
//	obj->lifecycle = EObjectLifecycle::beingDeleted;
//	ObjectSystem::instance().deleting.push_back(obj);
//}
//
//
//class IComponent :
//	public Object {
//public:
//};
//class Actor :
//	public Object {
//public:
//};


#endif // !_object


