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



class ObjectManager :
	public Singleton<ObjectManager>{
public:
	ScriptManager scripts;
	void tick() {
		scripts.tick();
	}
};

class gObject :
	public Scriptable {
	static ObjectManager& men;
public:
	virtual void enable() { 
		if (!isEnable) {
			Scriptable::enable();
			men.scripts << this;
		}
	}
	virtual void disable() {
		if (isEnable) {
			Scriptable::disable();
			men.scripts >> this;
		}
	}
	virtual void constructor() {
	}
	virtual void destructor() {
	}
	virtual void tick() {
	}

	gObject() {
		enable();
	}
	virtual ~gObject() {		
		disable();
	}
};
_declspec(selectany) ObjectManager& gObject::men = ObjectManager::instance();
#endif // !_memory