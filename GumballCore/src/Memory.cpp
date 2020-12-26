#include "Memory.hpp"

ObjectManager& ObjectManager::operator<<(gObject* obj) {
	allObjects.push_back(obj);
	return *this;
}
ObjectManager& ObjectManager::operator>>(gObject* obj) {
	toDelete.push_back(obj);
	return *this;
}
void ObjectManager::enableTick(gObject* obj) {
	tickObjects.push_back(obj);
}
void ObjectManager::disableTick(gObject* obj) {
	tickObjects.remove(obj);
}
void ObjectManager::tick() {
	for (auto& obj : toDelete) {
		allObjects.remove(obj);
		tickObjects.remove(obj);
		::delete obj;
	}
	for (auto& obj : tickObjects)
		obj->tick();

	toDelete.clear();
}