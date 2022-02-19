#include "Engine.hpp"
#include "EngineSystem.hpp"
#include "AssetManager.hpp"
#include "RenderSystem.hpp"
#include "EditorOverlay.hpp"
#include "ProjectLinker.hpp"

#include <iostream>
#include <string>

//#include "Object.hpp"
//#include "Event.hpp"
using namespace std;

//Engine * Engine::inst = nullptr;


template<class T> string getClassName(){
	string out = typeid(T).name();
	return out.substr(6, out.size() - 6);
}
//Systems.emplace(getClassName<AssetsSystem>(), &AssetsSystem::instance());


Engine::Engine() {
	project = new ProjectLinker;
	project->dllPath = "C:\\Users\\ADM\\source\\repos\\Base\\Gumball\\Build\\Debug\\GumballProject\\GumballProject.dll";


	RenderSystem *renderSystem = new RenderSystem;
	AssetsSystem *assetSystem = new AssetsSystem;
	InputSystem *inputSystem = new InputSystem;

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
	
	while (true) {
		if (project->hasToLoad()) {
			if (project->isLoaded()) {
				for (auto &s : systems)
					s->onEndplay();
			}
			project->load();
			if (project->isLoaded()) {
				for (auto &s : systems)
					s->onPlay();
			}
		}


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



//Engine::Engine() {
//	systems.push_back(&renderSystem);
//	systems.push_back(&assetSystem);
//	systems.push_back(&inputSystem);
//	systems.push_back(&objectSystem);
//
//	tickingSystems.push_back(&renderSystem);
//	tickingSystems.push_back(&inputSystem);
//	tickingSystems.push_back(&objectSystem);	
//}
//Engine::~Engine(){
//}
//void Engine::initialize() {
//	for (auto &s : systems)
//		s->initialize();
//
//	editor = dynamic_cast<EditorOverlay *>(renderSystem.getLayer("editor"));
//}
//void Engine::shutdown() {
//	for (auto& s : systems)
//		s->shutdown();
//}
//void Engine::onPlay() {
//	for (auto& s : systems)
//		s->onPlay();
//}
//void Engine::onEndplay() {
//	for (auto& s : systems)
//		s->onEndplay();
//}
//void Engine::tick(float deltaTime) {
//	static string names[] = { "render", "input", "object"};
//	timeStats.capture();
//	editor->msStats["fps"] = timeStats.getFPS();
//	
//	int id = 0;
//	TimeStat debugTimeStats;
//	for (auto &s : tickingSystems) {
//		debugTimeStats.capture();
//		s->tick(timeStats.getDeltaTime());
//		debugTimeStats.capture();
//		editor->msStats[names[id++]] = debugTimeStats.getMS();
//	}	
//}