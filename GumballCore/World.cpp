#include "World.hpp"
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
void GameObject::setTick(bool newTick) {
	static World &w = *Engine::instance()->world;
	w.scene.changeTick(this);
	hasTick = newTick;
}
void *GameObject::operator new(unsigned long long sz) {
	static World &w = *Engine::instance()->world;
	GameObject *ob = reinterpret_cast<GameObject *>(::operator new(sz));
	w.scene.root(ob);
	return ob;
}
void destroy(GameObject *trg) {
	static World &w = *Engine::instance()->world;
	w.scene.unRoot(trg);
}


void GameScene::unload() {
	toBegin.clear();
	toEnd.clear();
	toTick.clear();
	for (auto &e : entities) {
		delete e;
		e = nullptr;
	}
}
void GameScene::root(GameObject *go) {
	entities.emplace_back(go);
	go->state = EGameObjectState::spawn;
	toBegin.push_back(entities.back());
}
void GameScene::unRoot(GameObject *go) {
	if (go->isValid()) {
		toEnd.push_back(go);
		go->state = EGameObjectState::destroy;
	}
}
void GameScene::changeTick(GameObject *go) {
	toChangeTick.push_back(go);
}
Inline void GameScene::tick(const double &deltaTime) {
	if (toBegin.size()) {
		for (auto &e : toBegin) {
			e->beginPlay();
			e->state = EGameObjectState::nominal;
			if (e->getTick()) {
				toTick.push_back(e);
			}
		}
		toBegin.clear();
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
	if (toChangeTick.size()) {
		for (auto &e : toChangeTick) {
			if(e->isValid()) {
				if (e->getTick()) {
					toTick.push_back(e);
				}
				else {
					__erase(toTick, e);
				}
			}
		}
		toChangeTick.clear();
	}
	
	for (auto &e : toTick) {
		e->tick(deltaTime);
	}
}


World::World() {
}
void World::initialize() {
}
void World::shutdown() {
	scene.unload();
}
void World::beginPlay() {
}
void World::endPlay() {
	scene.unload();
}
void World::tick(const double &deltaTime) {
	scene.tick(deltaTime);
}