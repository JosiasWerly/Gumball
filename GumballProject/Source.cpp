//#include <GumballCore/Engine.hpp>
#include <GumballCore/ProjectLinker.hpp>
#include <iostream>
#include <map>
#include <string>
using namespace std;



class Calculator {
	typedef float (*Fnx)(float a, float b);
	map<string, Fnx> opr = {
		{
			"+",
			[](float a, float b)->float {
				return a + b; 
			}
		},
		{
			"*",
			[](float a, float b)->float {
				return a * b;
			}
		},
		{
			"-",
			[](float a, float b)->float {
				return a - b;
			}
		},
		{
			"/",
			[](float a, float b)->float {
				return a / b;
			}
		}
	};

public:
	Calculator() {
	}
	~Calculator() {
	}
	void proc() {
		cout << opr["/"](8, 4) << endl;
	}
};




Extern void OnProjectAttached(Engine &engineRef) {
	Engine::setInstance(&engineRef);
	cout << "<< loaded >> " << endl;
	cout << "&" << (long) &*Engine::instance() << endl;
	//Calculator c;
	//c.proc();
}
Extern void OnProjectDettached() {
	cout << ">> unloaded" << endl;
}



//auto &engine = Engine::instance();
//engine.initialize();
//engine.assetSystem.loadAllFiles("res\\");
//
//View v;
//v.viewMode.setProjectionPerspective();
//v.transform.position.z = -10;
//engine.renderSystem.pushView(0, &v);
//
//DrawInstance dd;
//dd.setMesh("cube");
//engine.renderSystem.pushDrawInstance(0, &dd);
//
//const float vel = 0.01f;
//
//engine.onPlay();
//while (1) {
//
//	if (engine.inputSystem.isKeyDown(Input::EKeyCode::W))
//		dd.transform.position += dd.transform.rotator.forward() * vel;
//	else if (engine.inputSystem.isKeyDown(Input::EKeyCode::S))
//		dd.transform.position -= dd.transform.rotator.forward() * vel;
//
//	if (engine.inputSystem.isKeyDown(Input::EKeyCode::D))
//		dd.transform.position += dd.transform.rotator.right() * vel;
//	else if (engine.inputSystem.isKeyDown(Input::EKeyCode::A))
//		dd.transform.position -= dd.transform.rotator.right() * vel;
//
//	if (engine.inputSystem.isKeyDown(Input::EKeyCode::UP))
//		dd.transform.rotator.rotate(1, 0, 0);
//	else if (engine.inputSystem.isKeyDown(Input::EKeyCode::DOWN))
//		dd.transform.rotator.rotate(-1, 0, 0);
//
//	if (engine.inputSystem.isKeyDown(Input::EKeyCode::LEFT))
//		dd.transform.rotator.rotate(0, 0, -1);
//	else if (engine.inputSystem.isKeyDown(Input::EKeyCode::RIGHT))
//		dd.transform.rotator.rotate(0, 0, 1);
//
//	engine.tick(0.f);
//}
//engine.shutdown();
//return 0;