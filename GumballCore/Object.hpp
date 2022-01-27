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
	//Object
		virtual ~Object() {}
	virtual string getName() = 0;
};

class ObjectSystem :
	public IEngineSystem,
	public Singleton<ObjectSystem> {
	list<Var<Object>>  objects;
public:
	Var<Object> newObject(Object* newObject) {
		//Var<Object> newInstance = newObject;
		//objects[newInstance->getAddress()].push_back(newInstance);
		return Var<Object>();
	}
	void destroyObject(Var<Object>& other) {
		delete* other;
	}
	Var<Object>* findObject(string name) {
		for (auto& obj : objects) {
			if (obj->getName() == name)
				return &obj;
		}
		return nullptr;
	}
	void tick() override {

	}
};

#endif // !_gameobject


