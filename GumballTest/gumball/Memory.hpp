#ifndef _memory
#define _memory
#include "Patterns.hpp"

template<class T> struct Dt {
public:
	T* value = nullptr;
	unsigned int refs = 1;
	Dt(T* init) :
		value(init) {
	}
	~Dt() {
		delete value;
	}
};
template<class T> class Var {
public:
	typedef Var<T> varT;
	typedef Dt<T> dtT;

	dtT* dt;
	Var(T* ptr = nullptr) {
		dt = new dtT(ptr ? ptr : new T);
	}
	Var(varT& Other) {
		dt = Other.dt;
		dt->refs++;
	}
	~Var() {
		if (dt && --dt->refs == 0) {
			delete dt;
		}
	}

	varT& operator=(varT& cpy) {
		if (dt != cpy.dt) {
			if (dt)
				dt->refs--;
			dt = cpy.dt;
			dt->refs++;
		}
		return *this;
	}

	operator T* () {
		return dt ? dt->value : nullptr;
	}
	T* operator=(T* newValue) {
		if (dt->value)
			delete dt->value;
		dt->value = newValue;
		return dt->value;
	}
	T* operator()() {
		return dt->value;
	}

	operator bool() {
		return dt->value;
	}
};


class gObject;
class ObjectManager :
	public Singleton<ObjectManager>{
	list<gObject*> 
		allObjects, //holds references to all objects
		tickObjects,  // holds references to object that will tick 
		toDelete; //holds references to objects that will be destroyed
public:
	ObjectManager& operator<<(gObject* obj);
	ObjectManager& operator>>(gObject* obj);
	void enableTick(gObject* obj);
	void disableTick(gObject* obj);
	void tick();

};

class gObject {
	static ObjectManager& objManager;
protected:
	bool isTicking, isEnable;
public:
	gObject() {
		objManager << this;
		if (this->getTick())
			objManager.enableTick(this);
	}
	void operator delete(void* obj) {
		objManager >> (gObject*)obj;
	}

	bool getEnable() { return isEnable; }
	bool getTick() { return isTicking; }

	virtual void setEnable(bool enable) {
		isEnable = enable;
	}
	virtual void setTick(bool enable) {
		isTicking = enable;
		if (enable)
			objManager.enableTick(this);
		else
			objManager.disableTick(this);
	}
	virtual void tick() {
	}
};
_declspec(selectany) ObjectManager& gObject::objManager = ObjectManager::instance();
#endif // !_memory