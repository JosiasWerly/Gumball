#include "World.hpp"
#include "Engine.hpp"

#include <string>


GameObject::GameObject() {
	setName("GameObject");
	state = EGameObjectState::begining;
}
GameObject::GameObject(string name) {
	setName(name);
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
	while (true) {
		bool validName = true;
		for (auto e : es.entities) {
			if (e->name == name) {
				string &n = e->name;
				int marker = n.find_last_of("_");
				if (marker == -1)
					name += "_1";
				else
					name = name.substr(0, marker) + string("_") + std::to_string(std::stoi(n.substr(marker + 1)) + 1);
				validName = false;
				break;
			}
		}
		if (validName)
			break;
	}
	this->name = name;
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
	toBegin.push_back(&entities.back());
}
void EntitySubsystem::del(GameObject *go) {
	for (list<Var<GameObject>>::iterator it = entities.begin(); it != entities.end(); ++it) {
		if (it->pin() == go) {
			toEnd.push_back(&(*it));
			break;
		}
	}
}
Inline void EntitySubsystem::tick(const double &deltaTime) {
	if (toBegin.size()) {
		for (auto e : toBegin) {
			(*e)->beginPlay();
			toTick.push_back(e);
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


Component::Component() : 
	owner(nullptr) {
}
Component::~Component() {

}

void *Actor::operator new(unsigned long long sz){
	static World &w = *Engine::instance()->world;
	Actor *newActor = reinterpret_cast<Actor*>(::operator new(sz));
	w.entitySystem->add(newActor);
	return newActor;
}
void Actor::operator delete(void *ptr) {
	static World &w = *Engine::instance()->world;
	w.entitySystem->del(reinterpret_cast<Actor *>(ptr));
}
Actor::Actor() {
}
Actor::~Actor() {
}
void Actor::beginPlay() {
	for (auto &c : components)
		c->beginPlay();
}
void Actor::endPlay() {
	for (auto &c : components)
		c->endPlay();
}
void Actor::tick(const double &deltaTime) {
	for (auto &c : components)
		c->tick(deltaTime);
}
void Actor::addComponent(Component *comp) {
	static World &w = *Engine::instance()->world;
	if (comp->owner)
		comp->owner->delComponent(comp);
	comp->owner = this;
	if (isValid())
		comp->beginPlay();
}
void Actor::delComponent(Component *comp) {
	if(comp->owner == this)
		components.remove(comp);
	if (isValid())
		comp->endPlay();
}
Component *Actor::findComponent(string name) {
	for (auto &c : components) {
		if (c->getName() == name)
			return c;
	}
	return nullptr;
}
template<class t> t *Actor::findComponent() {
	for (auto &c : components) {
		if (t *cc = dynamic_cast<t>(c))
			return cc;
	}
	return nullptr;
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
}
void World::tick(const double &deltaTime) {
	entitySystem->tick(deltaTime);
}
