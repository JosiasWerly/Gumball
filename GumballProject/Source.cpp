#include <iostream>
#include <map>
#include <string>
using namespace std;

#include <GumballCore/Definitions.hpp>
#include <GumballCore/ProjectLinker.hpp>
#include <GumballCore/SceneOverlay.hpp>
#include <GumballCore/WidgetOverlay.hpp>
#include <GumballCore/World.hpp>
#include <GumballCore/Actor.hpp>

class MyComp : 
	public MeshComponent {
public:
	Vector3 vel;
	MyComp() {
		setTick(true);
	}
	void beginPlay() {
		MeshComponent::beginPlay();
		owner->transform.position = Vector3(rand() % 9 - 5, rand() % 9 - 5, rand() % 9 - 5);
		owner->transform.rotator.rotate(90, 0, 0);
		vel = Vector3(rand() % 2 + 1, rand() % 2 + 1, rand() % 2 + 1);
		vel = vel.normalize() * 0.01;
		if (rand() % 2) {
			vel *= -1;
		}
	}
	void tick(const double &deltaTime) {
		//owner->transform.rotator.rotate(1, 0, 1);
		owner->transform.position += vel;
		if (abs(owner->transform.position.x) > 10) {
			owner->transform.position.x *= -1;
		}
		if (abs(owner->transform.position.y) > 10) {
			owner->transform.position.y *= -1;
		}
		if (abs(owner->transform.position.z) > 10) {
			owner->transform.position.z *= -1;
		}
	}
};


class MyActor :
	public Actor {
public:
	//DrawCallInstance *di;
	MyActor() {
		//di = new DrawCallInstance("torus");
		//di->transform = &transform;
		transform.scale = Vector3(0.001, 0.001, 0.001);
		addComponent(new MyComp);
	}
	virtual ~MyActor() {
		//delete di;
		//di = nullptr;
	}
	void beginPlay() override {
		Actor::beginPlay();
	}
	void endPlay() override {
		Actor::endPlay();
	}
	void tick(const double &deltaTime) {
		Actor::tick(deltaTime);
	}
};


class MyProject :
	public Project {
public:
	//when this DLL is attached
	virtual void attached() {
		srand(0);
		auto cameraActor = new Actor;
		{
			auto cameraComp = new CameraComponent;
			cameraActor->addComponent(cameraComp);
			cameraActor->transform.position.z = -30;
		}

		for (size_t i = 0; i < 2000; i++) {
			new MyActor;
		}
	}

	//when this DLL is detached
	virtual void detached() {

	}
};


Extern GBPROJECT void *EntryPoint() {
	return new MyProject;
}