#include <iostream>
#include <map>
#include <string>
using namespace std;

#include <GumballCore/SceneOverlay.hpp>
#include <GumballCore/WidgetOverlay.hpp>
#include <GumballCore/WorldSystem.hpp>
#include <GumballCore/Actor.hpp>


class Super {
public:
	Super() {
		cout << "Super ";
	}
};
class A : public Super {
public:
	A() {
		cout << "A" << endl;
	}
};
class B : public Super {
public:
	B() {
		cout << "B" << endl;
	}
};
class C : public Super {
public:
	C() {
		cout << "C" << endl;
	}
};

class MyComp :
	public MeshComponent {
public:
	Vector3 vel;
	MyComp() {
		setTickEnable(true);
	}
	void beginPlay() {
		MeshComponent::beginPlay();
		vel = Vector3(rand() % 2 + 1, rand() % 2 + 1, rand() % 2 + 1);
		vel = vel.normalize() * 0.01;
		if (rand() % 2) {
			vel *= -1;
		}
	}
	void tick(const double &deltaTime) {
		owner->transform.rotator.rotate(0, .5, 0);
		return;
		owner->transform.position += vel;
		auto &pos = owner->transform.position;
		if (pos.x < -10) {
			pos.x = -10;
			vel.x *= -1;
		}
		else if (pos.x > 10) {
			pos.x = 10;
			vel.x *= -1;
		}

		if (pos.y < -10) {
			pos.y = -10;
			vel.y *= -1;
		}
		else if (pos.y > 10) {
			pos.y = 10;
			vel.y *= -1;
		}
		if (pos.z < -10) {
			pos.z = -10;
			vel.z *= -1;
		}
		else if (pos.z > 10) {
			pos.z = 10;
			vel.z *= -1;
		}
	}
};

static int i = 0;
class MyActor :
	public Actor {
public:
	//DrawCallInstance *di;
	MyActor() {
		//di = new DrawCallInstance("torus");
		//di->transform = &transform;
		transform.scale = Vector3(0.001, 0.001, 0.001);
		transform.position = Vector3(-5 + i, 0, 0);
		i += 3;
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