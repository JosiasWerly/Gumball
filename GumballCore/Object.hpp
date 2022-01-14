#pragma once
#ifndef _object
#define _object

#include <iostream>
#include <list>
#include <map>
#include <unordered_map>
using namespace std;

#include "EngineSystem.hpp"
#include "Patterns.hpp"

typedef long long ObjAddrs;
class Object {
public:
	virtual ~Object() {}

	virtual string getName() = 0;
	virtual ObjAddrs getAddress() = 0;
};

#define ObjectImpl(Target)\
virtual string getName() override { return #Target; }\
virtual ObjAddrs getAddress() override { return Target; }

class ObjectSystem :
	public Singleton<ObjectSystem> {
	unordered_map<ObjAddrs, list<Var<Object>>>  objects;
public:
	Var<Object> newObject(Object* newObject) {
		//Var<Object> newInstance = newObject;
		//objects[newInstance->getAddress()].push_back(newInstance);
		return Var<Object>();
	}
	void destroyObject(Var<Object>& other) {
		delete *other;
	}
	Var<Object>* getObject(ObjAddrs address, string name) {
		if (objects.contains(address)) {
			for (auto &obj : objects[address]) {
				if (obj->getName() == name)
					return &obj;
			}
		}
		return nullptr;
	}
};

#endif // !_gameobject


