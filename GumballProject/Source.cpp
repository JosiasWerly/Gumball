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



class MyActor :
	public Actor {
public:
	Vector3 vel;
	DrawCallInstance *di;
	MyActor() {
		di = new DrawCallInstance("torus");
		di->transform = &transform;
		transform.position = Vector3(rand() % 10, rand() % 10, rand() % 10);
		vel = Vector3(rand() % 2 + 1, rand() % 2 + 1, rand() % 2 + 1);
		vel = vel.normalize() * 0.2;
		if (rand() % 2) {
			vel *= -1;
		}
		transform.scale = Vector3(0.001, 0.001, 0.001);
	}
	virtual ~MyActor() {
		delete di;
		di = nullptr;
	}
	void beginPlay() override {
		Actor::beginPlay();
	}
	void endPlay() override {
		Actor::endPlay();
	}
	void tick(const double &deltaTime) {
		Actor::tick(deltaTime);
		transform.rotator.rotate(1, 0, 0);
		transform.position += vel;
		if (abs(transform.position.x) > 5) {
			transform.position.x *= -1;
		}
		if (abs(transform.position.y) > 5) {
			transform.position.y *= -1;
		}
		if (abs(transform.position.z) > 5) {
			transform.position.z *= -1;
		}
	}
};


class MyProject :
	public Project {
public:
	//when this DLL is attached
	virtual void attached() {
		srand(0);
		auto w = Engine::instance()->world;
		for (size_t i = 0; i < 1000; i++) {
			new MyActor();
		}
	}

	//when this DLL is detached
	virtual void detached() {

	}
};


Extern GBPROJECT void *EntryPoint() {	
	return new MyProject;
}