#include <iostream>
#include <map>
#include <string>
using namespace std;


#include <GumballCore/ProjectLinker.hpp>
#include <GumballCore/SceneOverlay.hpp>
#include <GumballCore/WidgetOverlay.hpp>

class MyProject :
	public Project {
public:
	DrawInstance *a;

	void initialize() {
		auto e = Engine::instance();
		auto &scene = *dynamic_cast<SceneOverlay *>(e->renderSystem->getLayer("scene"));

		a = new DrawInstance;
		a->setMesh("cube");
		a->setTexture("scotty");
		a->transform.position.x = -1;
		scene.pushDrawInstance(a);
	}
	void shutdown() {
		auto &scene = *dynamic_cast<SceneOverlay *>(Engine::instance()->renderSystem->getLayer("scene"));
		scene.popDrawInstance(a);
	}
	void tick(float deltaTime) {
		auto inputSystem = Engine::instance()->inputSystem;
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
	}
};

Extern GBPROJECT void *EntryPoint(Engine &inst) {
	return new MyProject;
}