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



static int id = 0;
class MyActor :
	public Actor {
public:
	int myId = 0;
	DrawCallInstance *di;

	MyActor() {
		di = new DrawCallInstance("torus");
		di->transform = &transform;
	}
	virtual ~MyActor() {
		delete di;
		di = nullptr;
	}
	void beginPlay() override {
		Actor::beginPlay();
		cout << getName() << " beg" << endl;
	}
	void endPlay() override {
		Actor::endPlay();
		cout << getName() << " end" << endl;
	}
	void tick(const double &deltaTime) {
		Actor::tick(deltaTime);
		auto inputSystem = Engine::instance()->inputSystem;
		if (inputSystem->isKeyDown(Input::EKeyCode::UP))
			transform.rotator.rotate(1, 0, 0);
		else if (inputSystem->isKeyDown(Input::EKeyCode::DOWN))
			transform.rotator.rotate(-1, 0, 0);
		
		if (inputSystem->isKeyDown(Input::EKeyCode::LEFT))
			transform.rotator.rotate(0, 0, -1);
		else if (inputSystem->isKeyDown(Input::EKeyCode::RIGHT))
			transform.rotator.rotate(0, 0, 1);
		
		
		if (inputSystem->onKeyReleased(Input::EKeyCode::Q)) {
			auto newActor = new MyActor();
			newActor->transform.position = transform.position + Vector3(0.4, 0, 0);
			newActor->myId = ++id;
		}
		else if (inputSystem->onKeyReleased(Input::EKeyCode::W)) {
			if (myId == id) {				
				destroy(this);
				--id;
			}
		}
	}
};


class MyProject :
	public Project {
public:
	//when this DLL is attached
	virtual void attached() {
		auto w = Engine::instance()->world;
		new MyActor();
	}

	//when this DLL is detached
	virtual void detached() {

	}
};


Extern GBPROJECT void *EntryPoint() {	
	return new MyProject;
}