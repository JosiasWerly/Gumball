#include "World.hpp"
#include "Engine.hpp"

#include <string>


GameObject::GameObject() {
	setName("GameObject");
}
GameObject::~GameObject() {
}
void GameObject::beginPlay() {
}
void GameObject::endPlay() {
}
void GameObject::tick(const double &deltaTime) {
}
string GameObject::getName() {
	return name;
}
void GameObject::setName(string name) {
	static EntitySubsystem &es = *Engine::instance()->world->entitySystem;
	this->name = es.requestName(name);
}
void *GameObject::operator new(unsigned long long sz) {
	static World &w = *Engine::instance()->world;
	GameObject *newGo = reinterpret_cast<GameObject *>(::operator new(sz));
	w.entitySystem->add(newGo);
	return newGo;
}
void GameObject::destroy(GameObject *go) {
	static World &w = *Engine::instance()->world;
	w.entitySystem->del(go);
}


string EntitySubsystem::requestName(const string &suggestedName) {
	string name = suggestedName;
	bool searchingName;
	do {
		searchingName = false;
		for (auto e : entities) {
			string &n = e->name;
			if (n == name) {
				searchingName = true;
				int marker = n.find_last_of("_");
				if (marker == -1)
					name += "_1";
				else
					name = name.substr(0, marker) + string("_") + std::to_string(std::stoi(n.substr(marker + 1)) + 1);
				break;
			}
		}
	} while (searchingName);
	return name;
}
void EntitySubsystem::unload() {
	toBegin.clear();
	toEnd.clear();
	toTick.clear();
	for (auto e : entities)
		e.free();
}
void EntitySubsystem::add(GameObject *go) {
	entities.emplace_back(Var<GameObject>(go));
	go->state = EGameObjectState::spawn;
	toBegin.push_back(&entities.back());
}
void EntitySubsystem::del(GameObject *go) {
	for (auto &it : entities) {
		if (it.pin() == go) {
			if (it->state != EGameObjectState::destroy) {
				it->state = EGameObjectState::destroy;
				toEnd.push_back(&it);
				break;
			}
		}
	}
}
Inline void EntitySubsystem::tick(const double &deltaTime) {
	if (toBegin.size()) {
		for (auto e : toBegin) {
			(*e)->beginPlay();
			toTick.push_back(e);
			(*e)->state = EGameObjectState::nominal;
		}
		toBegin.clear();
	}
	if (toEnd.size()) {
		for (auto e : toEnd) {
			toTick.remove(e);
			e->pin()->endPlay();
			(*e).free();
		}
		toEnd.clear();
	}
	for (auto &e : toTick) {
		(*e)->tick(deltaTime);
	}
}

World::World() {
	entitySystem = new EntitySubsystem;
}
void World::initialize() {
}
void World::shutdown() {
	entitySystem->unload();
}
void World::beginPlay() {
}
void World::endPlay() {
	entitySystem->unload();
}
void World::tick(const double &deltaTime) {
	entitySystem->tick(deltaTime);
}
