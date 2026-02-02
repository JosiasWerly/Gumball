#include "Sandbox.hpp"

#include <Asset/Asset.module.hpp>
#include <Render/Render.module.hpp>
#include <Gumball/Domain.hpp>
#include <Gumball/Engine.hpp>

#include <Render/Scene/SceneOverlay.hpp>
#include <Render/Widget/WidgetOverlay.hpp>
#include <Input/Input.module.hpp>
#include <World/Actor.hpp>

#include <Gumball/Flow/Common.hpp>
#include <Gumball/Containers/Pointer.hpp>

#include <iostream>
#include <list>
#include <memory>
#include <functional>

#include <chrono>
#include <thread>
#include <mutex>
#include <future>



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
		vel = vel.normalize() * 0.01f;
		if (rand() % 2) {
			vel *= -1;
		}
	}
	void tick(const double &deltaTime) {
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		owner->transform.rotation.rotate(.05f, .05f, .05f);
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
		transform.scale = Vector3(int(0.001), int(0.001), int(0.001));
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

#include <Gumball/Concurrent/Scheduler.hpp>
using namespace Concurrent;

static Mutex p;
#define Trace { \
GuardUnique trc_ln (p);\
std::cout << this_thread::get_id() << " " << __FUNCTION__ << std::endl;\
this_thread::sleep_for(std::chrono::duration<long double, std::milli>(500)); }

static int arr[1024];

static list<char> renderRequest;
static list<string> renderInst;
bool Render_Request() {
	Trace;
	if (!renderRequest.empty()) {
		renderRequest.emplace_back();
		
		while(!renderRequest.empty()){
			renderRequest.back() += renderRequest.front();
			renderRequest.pop_front();
		}
	}
	return renderInst.empty();
}
bool Render_Order() {
	Trace;	
	return true;
}
bool Render_Draw() {
	Trace;
	return true;
}

bool Gameplay() {
	Trace;
	return true;
}
bool Gameplay_A() {
	Trace;
	return true;
}
bool Gameplay_B() {
	Trace;
	return true;
}
Extern GGAME void *EntryPoint() {
	Scheduler sc;
	{
		Job render("render");
		Task &request = render.add("request");
		request.fn.bind(&Render_Request);

		Task &order = render.add("order");
		order.fn.bind(&Render_Order);

		Task &draw = render.add("draw");
		draw.fn.bind(&Render_Draw);

		render.begin.bind([&]()->bool { Trace; return false; });
		render.end.bind([&]()->bool { Trace;  sc.pop(render); return false; });
		sc.add(render);

		Job gameplay("gameplay");

		Task &A = gameplay.add("tick");
		A.fn.bind(&Gameplay_A);

		Task &B = gameplay.add("tock");
		B.fn.bind(&Gameplay_B);
		gameplay.end.bind([&]()->bool { Trace;  sc.pop(gameplay); return false; });
		sc.add(gameplay);

		sc.start(4);
		while (true);
	}
	cout << "work complete" << endl;
	return new MyProject;
}