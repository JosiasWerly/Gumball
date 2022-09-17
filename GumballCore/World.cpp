#include "World.hpp"
#include "Engine.hpp"

void World::registerActor(Actor *actor) {
	actors.emplace_back(Var<Actor>(actor));
	toSpawn.push_back(&actors.back());
}
void World::unregisterActor(Actor *actor) {
	for (list<Var<Actor>>::iterator it = actors.begin(); it != actors.end(); ++it) {
		if (it->pin() == actor) {
			toDelete.push_back(&(*it));
			break;
		}
	}
}
void World::initialize() {
}
void World::shutdown() {
	toSpawn.clear();
	toDelete.clear();
	toTick.clear();
	for (auto a : actors)
		a.free();
}
void World::beginPlay() {
}
void World::endPlay() {
}
void World::tick(const double &deltaTime) {
	if (toSpawn.size()) {
		for (auto a : toSpawn) {
			(*a)->beginPlay();
			toTick.push_back(a);
		}
		toSpawn.clear();
	}
	if (toDelete.size()) {
		for (auto a : toDelete) {
			toTick.remove(a);
			(*a)->endPlay();
			(*a).free();
		}
		toDelete.clear();
	}
	for (auto &a : toTick)
		(*a)->tick(deltaTime);
}


void *Actor::operator new(unsigned long long sz){
	static World &w = *Engine::instance()->world;
	Actor *newActor = reinterpret_cast<Actor*>(::operator new(sz));
	w.registerActor(newActor);	
	return newActor;
}
void Actor::operator delete(void *ptr) {
	static World &w = *Engine::instance()->world;
	w.unregisterActor(reinterpret_cast<Actor *>(ptr));
}
Actor::Actor() {
}
Actor::~Actor() {
}