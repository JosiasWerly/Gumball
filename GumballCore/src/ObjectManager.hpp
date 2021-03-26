#ifndef _memory
#define _memory
#include "Patterns.hpp"



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