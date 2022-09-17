#include <iostream>
#include <map>
#include <string>
using namespace std;


#include <GumballCore/ProjectLinker.hpp>
#include <GumballCore/SceneOverlay.hpp>
#include <GumballCore/WidgetOverlay.hpp>
#include <GumballCore/World.hpp>

class MyProject :
	public Project {
public:
	//DrawInstance *a;

	void initialize() {
		/*auto e = Engine::instance();
		auto &scene = *dynamic_cast<SceneOverlay *>(e->renderSystem->getLayer("scene"));

		a = new DrawInstance;
		a->setMesh("torus");
		a->setTexture("logo");
		a->transform.position.x = -1;
		scene.pushDrawInstance(a);*/
	}
	void shutdown() {
		/*auto &scene = *dynamic_cast<SceneOverlay *>(Engine::instance()->renderSystem->getLayer("scene"));
		scene.popDrawInstance(a);*/
	}
	void tick(const double &deltaTime) {
		/*auto inputSystem = Engine::instance()->inputSystem;
		if (inputSystem->isKeyDown(Input::EKeyCode::W))
			a->transform.position += a->transform.rotator.forward() * .5;
		else if (inputSystem->isKeyDown(Input::EKeyCode::S))
			a->transform.position -= a->transform.rotator.forward() * .5;

		if (inputSystem->isKeyDown(Input::EKeyCode::D))
			a->transform.position += a->transform.rotator.right() * .5;
		else if (inputSystem->isKeyDown(Input::EKeyCode::A))
			a->transform.position -= a->transform.rotator.right() * .5;

		if (inputSystem->isKeyDown(Input::EKeyCode::UP))
			a->transform.rotator.rotate(1, 0, 0);
		else if (inputSystem->isKeyDown(Input::EKeyCode::DOWN))
			a->transform.rotator.rotate(-1, 0, 0);

		if (inputSystem->isKeyDown(Input::EKeyCode::LEFT))
			a->transform.rotator.rotate(0, 0, -1);
		else if (inputSystem->isKeyDown(Input::EKeyCode::RIGHT))
			a->transform.rotator.rotate(0, 0, 1);
		*/
	}
};

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


Extern GBPROJECT void *EntryPoint() {
	new MyActor;
	return new MyProject;
}