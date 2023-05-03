#include "Actor.hpp"
#include "Engine.hpp"

ActorComponent::ActorComponent() :
	owner(nullptr) {
	setTick(false);
}
ActorComponent::~ActorComponent() {

}
void ActorComponent::activate() {
}
void ActorComponent::deactivate() {
}
void ActorComponent::newOwner(Actor *newOwner, Actor *oldOwner) {
}
void ActorComponent::beginPlay() {
	activate();
}
void ActorComponent::endPlay() {
	deactivate();
}
void ActorComponent::tick(const double &deltaTime) {
}
void ActorComponent::setActive(bool newActive) {
	if (newActive != isActive) {
		isActive = newActive;
		if (isActive) {
			activate();
		}
		else {
			deactivate();
		}
	}
}

MeshComponent::MeshComponent() {
	auto as = Engine::instance()->assetSystem;
	draw.setShader(as->getContent<Shader>("default"));
	draw.setMesh(as->getContent<MeshData>("cube"));
	
}
MeshComponent::~MeshComponent() {

}
void MeshComponent::activate() {
	draw.enable();
}
void MeshComponent::deactivate() {
	draw.disable();
}
void MeshComponent::newOwner(Actor *newOwner, Actor *oldOwner) {
	if (newOwner) {
		draw.transform = &newOwner->transform;
	}
	else {
		draw.transform = nullptr;
		deactivate();
	}
}

CameraComponent::CameraComponent() {
	view.viewMode.setProjectionPerspective();
}
CameraComponent::~CameraComponent() {

}
void CameraComponent::activate() {
	view.enable();
}
void CameraComponent::deactivate() {
	view.disable();
}
void CameraComponent::newOwner(Actor *newOwner, Actor *oldOwner) {
	if (newOwner) {
		view.transform = &newOwner->transform;
	}
	else {
		view.transform = nullptr;
		deactivate();
	}
}


Actor::Actor() {
}
Actor::~Actor() {
}
void Actor::beginPlay() {
}
void Actor::endPlay() {
}
void Actor::tick(const double &deltaTime) {
}
void Actor::addComponent(ActorComponent *comp) {
	static World &w = *Engine::instance()->world;
	Actor *oldOwner = comp->owner;
	if (oldOwner) {
		oldOwner->components.remove(comp);
	}
	comp->owner = this;
	comp->newOwner(this, oldOwner);
	components.push_back(comp);
}
void Actor::delComponent(ActorComponent *comp) {
	if (comp->owner == this) {
		components.remove(comp);
		destroy(comp);
	}
}
template<class t> t *Actor::findComponent() {
	for (auto &c : components) {
		if (t *cc = dynamic_cast<t>(c))
			return cc;
	}
	return nullptr;
}