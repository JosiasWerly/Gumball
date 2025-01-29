#include "Sandbox.hpp"

#include <Asset/Asset.module.hpp>
#include <Render/Render.module.hpp>
#include <Gumball/Domain.hpp>
#include <Gumball/Engine.hpp>

#include <Render/Scene/SceneOverlay.hpp>
#include <Render/Widget/WidgetOverlay.hpp>
#include <Input/Input.module.hpp>
#include <World/Actor.hpp>


#include <iostream>
#include <list>
#include <memory>
#include <functional>

class GGAME MyComp : public MeshComponent {
public:
	Vector3 vel;
	MyComp() {
		setTickEnable(true);
	}
	void beginPlay() {
		MeshComponent::beginPlay();
		owner->transform.scale = Vector3(1, 1, 1);
		vel = Vector3(rand() % 2 + 1, rand() % 2 + 1, rand() % 2 + 1);
		vel = vel.normalize() * 0.01;
		if (rand() % 2) {
			vel *= -1;
		}
	}
	void tick(const double &deltaTime) {
		owner->transform.rotation.rotate(.05, .05, .05);
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
static Actor *camera = nullptr;
class GGAME MyActor :
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
		setTickEnable(true);
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
		
		if (Input::isKeyDown(Input::EKeyCode::A)) {
			camera->transform.rotation.rotate(0, 0, 1.);
		}
		else if (Input::isKeyDown(Input::EKeyCode::D)) {
			camera->transform.rotation.rotate(0, 0, -1.);
		}
		if (Input::isKeyDown(Input::EKeyCode::W)) {
			camera->transform.rotation.rotate(0, 1, 0);
		}
		else if (Input::isKeyDown(Input::EKeyCode::S)) {
			camera->transform.rotation.rotate(0, -1, 0);
		}
	}
};

class GGAME MyProject : public Project {
public:	
	void attached() {
		cout << "loaded" << endl;
		srand(0);

		AssetModule *m = AssetModule::instance();
		WorldModule *w = WorldModule::instance();
		RenderModule *r = RenderModule::instance();
		InputModule *i = InputModule::instance();
		Domain *d = Domain::instance();

		auto cameraActor = new Actor;
		camera = cameraActor;
		{
			auto cameraComp = new CameraComponent;
			cameraActor->addComponent(cameraComp);
			cameraActor->transform.position.z = -20;
		}

		for (size_t i = 0; i < 5; i++) {
			new MyActor;
		}
	}
	void detached() {
		cout << "unloaded" << endl;
	}
};

Extern GGAME void *EntryPoint() {
	return new MyProject;
}