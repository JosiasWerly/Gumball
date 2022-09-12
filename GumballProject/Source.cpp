#include <iostream>
#include <map>
#include <string>
using namespace std;


#include <GumballCore/ProjectLinker.hpp>
#include <GumballCore/SceneOverlay.hpp>
#include <GumballCore/WidgetOverlay.hpp>


class Project :
	public IProject {
public:
	View *v;
	DrawInstance *a, *b;
	void onAttach(Engine &engine) {
		Engine::setInstance(&engine);
		auto &scene = *dynamic_cast<SceneOverlay*>(engine.getSystem<RenderSystem>()->getLayer("scene"));


		v = new View;
		v->viewMode.setProjectionPerspective();
		v->transform.position.z = -10;
		scene.pushView(v);

		a = new DrawInstance;
		a->setMesh("cube");
		a->setTexture("logo");
		a->transform.position.x = -1;
		scene.pushDrawInstance(a);
	}
	void onDettach() {
		auto scene = dynamic_cast<SceneOverlay *>(Engine::instance()->getSystem<RenderSystem>()->getLayer("scene"));
		scene->popView(v);
		scene->popDrawInstance(a);
		delete v;
		delete a;
	}
};

Extern IProject *OnProjectAttached() {
	return new Project;		
}