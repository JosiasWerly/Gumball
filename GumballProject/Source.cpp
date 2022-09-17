#include <iostream>
#include <map>
#include <string>
using namespace std;

#include <GumballCore/Definitions.hpp>
#include <GumballCore/ProjectLinker.hpp>
#include <GumballCore/SceneOverlay.hpp>
#include <GumballCore/WidgetOverlay.hpp>
#include <GumballCore/World.hpp>



static int id = 0;
class MyActor :
	public Actor {
public:
	int myId = 0;
	DrawInstance a;

	MyActor() {
		auto e = Engine::instance();
		auto &scene = *dynamic_cast<SceneOverlay *>(e->renderSystem->getLayer("scene"));
		a.setMesh("torus");
		a.setTexture("logo");
		scene.pushDrawInstance(&a);
	}
	~MyActor() {
		auto &scene = *dynamic_cast<SceneOverlay *>(Engine::instance()->renderSystem->getLayer("scene"));
		scene.popDrawInstance(&a);
	}
	void beginPlay() {

	}
	void endPlay() {
	
	}
	void tick(const double &deltaTime) {
		auto inputSystem = Engine::instance()->inputSystem;
		if (inputSystem->isKeyDown(Input::EKeyCode::UP))
			a.transform.rotator.rotate(1, 0, 0);
		else if (inputSystem->isKeyDown(Input::EKeyCode::DOWN))
			a.transform.rotator.rotate(-1, 0, 0);

		if (inputSystem->isKeyDown(Input::EKeyCode::LEFT))
			a.transform.rotator.rotate(0, 0, -1);
		else if (inputSystem->isKeyDown(Input::EKeyCode::RIGHT))
			a.transform.rotator.rotate(0, 0, 1);


		if (inputSystem->onKeyReleased(Input::EKeyCode::Q)) {
			auto newActor = new MyActor();
			newActor->a.transform.position = a.transform.position + Vector3(0.4, 0, 0);
			newActor->myId = ++id;
		}
		else if (inputSystem->onKeyReleased(Input::EKeyCode::W)) {
			if (myId == id) {
				delete this;
				--id;
			}
		}
	}
};



class MyProject :
	public Project {
public:
	//when this DLL is attached
	virtual void attached() {}

	//when this DLL is detached
	virtual void detached() {}
};


Extern GBPROJECT void *EntryPoint() {
	new MyActor;
	return new MyProject;
}