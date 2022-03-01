#include "Engine.hpp"
#include "EngineSystem.hpp"
#include "AssetManager.hpp"
#include "RenderSystem.hpp"
#include "EditorOverlay.hpp"
#include "ProjectLinker.hpp"

#include <iostream>
#include <string>


using namespace std;
//template<class T> string getClassName(){
//	string out = typeid(T).name();
//	return out.substr(6, out.size() - 6);
//}
//Systems.emplace(getClassName<AssetsSystem>(), &AssetsSystem::instance());


Engine::Engine() {
	project = new ProjectLinker;
	project->dllPath = "C:\\Users\\ADM\\source\\repos\\Base\\Gumball\\Build\\Debug\\GumballProject\\GumballProject.dll";


	renderSystem = new RenderSystem;
	assetSystem = new AssetsSystem;
	inputSystem = new InputSystem;

	systems.push_back(renderSystem);
	systems.push_back(assetSystem);
	systems.push_back(inputSystem);
	//systems.push_back(objectSystem);

	tickingSystems.push_back(renderSystem);
	tickingSystems.push_back(inputSystem);
	//tickingSystems.push_back(objectSystem);	
}
Engine::~Engine(){
}
void Engine::args(int argc, char *argv[]) {
	project->enginePath = argv[0];
	project->enginePath = project->enginePath.substr(0, project->enginePath.find_last_of("\\"));
}






void Engine::tick() {
	for (auto &s : systems)
		s->initialize();
	
	cout << "&" << (long)&*Engine::instance() << endl;	
	editor = dynamic_cast<EditorOverlay *>(getSystem<RenderSystem>()->getLayer("editor"));	
	
	
	assetSystem->loadAssetsFromFolder("res\\");
	
	View v;
	v.viewMode.setProjectionPerspective();
	v.transform.position.z = -10;
	renderSystem->pushView(0, &v);

	DrawInstance dd;
	dd.setMesh("cube");
	renderSystem->pushDrawInstance(0, &dd);

	while (true) {
		//if (project->hasToLoad()) {
		//	if (project->isLoaded()) {
		//		for (auto &s : systems)
		//			s->onEndplay();
		//	}
		//	project->load();
		//	if (project->isLoaded()) {
		//		for (auto &s : systems)
		//			s->onPlay();
		//	}
		//}


		static string names[] = { "render", "input", "object" };
		timeStats.capture();
		editor->msStats["fps"] = timeStats.getFPS();

		int id = 0;
		TimeStat debugTimeStats;
		for (auto &s : tickingSystems) {
			debugTimeStats.capture();
			s->tick((float)timeStats.getDeltaTime());
			debugTimeStats.capture();
			editor->msStats[names[id++]] = debugTimeStats.getMS();
		}
	}

	for (auto &s : systems) 
		s->onEndplay();

	for (auto &s : systems)
		s->shutdown();
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