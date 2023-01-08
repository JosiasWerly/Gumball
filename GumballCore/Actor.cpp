#include "Actor.hpp"
#include "Engine.hpp"

Component::Component() :
	owner(nullptr) {
}
Component::~Component() {

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
	if (comp->owner == this)
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


