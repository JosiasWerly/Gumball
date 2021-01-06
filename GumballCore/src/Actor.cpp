#include "Actor.hpp"

void Actor::setEnable(bool enable) {
	gObject::setEnable(enable);
	for (auto& ac : components)
		ac->setEnable(enable);
}
Actor& Actor::operator<<(ActorComponent* comp){
	components.insert(comp);
	comp->attachOwner(this);
	return *this;
}
Actor& Actor::operator>>(ActorComponent* comp) {
	components.erase(comp);
	comp->detachOwner();
	return *this;
}