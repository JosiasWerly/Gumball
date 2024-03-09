#include "WorldSystem.hpp"
#include "Engine.hpp"

#include <string>


GameObject::GameObject() {
}
GameObject::~GameObject() {
}
void GameObject::beginPlay() {
}
void GameObject::endPlay() {
}
void GameObject::tick(const double &deltaTime) {
}
void GameObject::setTickEnable(bool newState) {
	if (newState == tickEnable)
		return;
	tickEnable = newState;
	static WorldSystem &w = *Engine::instance()->worldSystem;
	w.entityController.changeTick(this);
}
void GameObject::setTickInterval(float newValue) {
	tickInterval = newValue;
}
void *GameObject::operator new(unsigned long long sz) {
	static WorldSystem &w = *Engine::instance()->worldSystem;
	GameObject *go = reinterpret_cast<GameObject *>(::operator new(sz));
	w.entityController.root(go);
	return go;
}
void destroy(GameObject *trg) {
	static WorldSystem &w = *Engine::instance()->worldSystem;
	w.entityController.unRoot(trg);
}


void WorldEntityController::unload() {
	toBegin.clear();
	toEnd.clear();
	toTick.clear();
	for (auto &e : entities) {
		delete e;
		e = nullptr;
	}
	entities.clear();
}
void WorldEntityController::root(GameObject *go) {
	entities.emplace_back(go);
	go->state = EGameObjectState::spawn;
	toBegin.push_back(entities.back());
}
void WorldEntityController::unRoot(GameObject *go) {
	if (!go->isValid())
		return;

	toEnd.push_back(go);
	go->state = EGameObjectState::destroy;
	if (go->tickEnable) {
		go->tickEnable = false;
		changeTick(go);
	}
}
void WorldEntityController::changeTick(GameObject *go) {
	toChangeTick.push_back(go);
}
void WorldEntityController::tick(const double &deltaTime) {
	if (toBegin.size()) {
		for (auto &e : toBegin) {
			e->beginPlay();
			e->state = EGameObjectState::nominal;
		}
		toBegin.clear();
	}
	if (toChangeTick.size()) {
		for (auto &e : toChangeTick) {
			if(e->isValid()) {
				if (e->getTickEnable()) {
					toTick.push_back(e);
				}
				else {
					__erase(toTick, e);
				}
			}
		}
		toChangeTick.clear();
	}
	if (toEnd.size()) {
		for (auto &e : toEnd) {
			e->endPlay();
			__erase(toTick, e);
			delete e;
			e = nullptr;
		}
		toEnd.clear();
	}
	
	for (auto &e : toTick) {
		e->tick(deltaTime);
	}
}


WorldSystem::WorldSystem() {
}
void WorldSystem::initialize() {
}
void WorldSystem::shutdown() {
	entityController.unload();
}
void WorldSystem::beginPlay() {
	//for (auto e : entityController.entities) {
	//	
	//}
}
void WorldSystem::endPlay() {

}
void WorldSystem::tick(const double &deltaTime) {
	entityController.tick(deltaTime);
}