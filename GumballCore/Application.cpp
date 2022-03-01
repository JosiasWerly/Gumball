#include "Engine.hpp"

#include <iostream>
using namespace std;




#include "Object.hpp"
#include "Var.hpp"



class Foo : 
	public Object {
public:
	int a;
	float b;

	void test() override {
		std::cout << a << b << std::endl;
	}
};

int main(int argc, char *argv[]) {
	Var<Object> b;
	{
		Var<Object> a, c = b;
		a = new Foo;
		b = a;
		{
			Var<Foo> af = a;
			a.As<Foo>()->test();
		}
	}
	
	//Var<Object> obj;
	//obj = new Foo;
	




	Engine::setInstance(new Engine);
	Engine::inst->args(argc, argv);
	Engine::inst->tick();
	return 0;
}




/*
	auto &engine = Engine::instance();
	engine.initialize();
	engine.assetSystem.loadAllFiles("res\\");
	
	View v;
	v.viewMode.setProjectionPerspective();
	v.transform.position.z = -10;
	engine.renderSystem.pushView(0, &v);
	 
	DrawInstance dd;
	dd.setMesh("cube");
	engine.renderSystem.pushDrawInstance(0, &dd);

	const float vel = 0.01f;

	engine.onPlay();
	while (1) {

		if (engine.inputSystem.isKeyDown(Input::EKeyCode::W))
			dd.transform.position += dd.transform.rotator.forward() * vel;
		else if (engine.inputSystem.isKeyDown(Input::EKeyCode::S))
			dd.transform.position -= dd.transform.rotator.forward() * vel;

		if (engine.inputSystem.isKeyDown(Input::EKeyCode::D))
			dd.transform.position += dd.transform.rotator.right() * vel;
		else if (engine.inputSystem.isKeyDown(Input::EKeyCode::A))
			dd.transform.position -= dd.transform.rotator.right() * vel;

		if (engine.inputSystem.isKeyDown(Input::EKeyCode::UP))
			dd.transform.rotator.rotate(1, 0, 0);
		else if (engine.inputSystem.isKeyDown(Input::EKeyCode::DOWN))
			dd.transform.rotator.rotate(-1, 0, 0);

		if (engine.inputSystem.isKeyDown(Input::EKeyCode::LEFT))
			dd.transform.rotator.rotate(0, 0, -1);
		else if (engine.inputSystem.isKeyDown(Input::EKeyCode::RIGHT))
			dd.transform.rotator.rotate(0, 0, 1);

		engine.tick(0.f);
	}
	engine.shutdown();
	return 0;
*/