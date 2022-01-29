#include "Engine.hpp"

#include <string>
using namespace std;

template<class T> string getClassName(){
	string out = typeid(T).name();
	return out.substr(6, out.size() - 6);
}
//Systems.emplace(getClassName<AssetsSystem>(), &AssetsSystem::instance());



Engine::Engine() {

	//RenderSystem &renderSystem = RenderSystem::instance();
	//AssetsSystem &assetSystem = AssetsSystem::instance();
	//InputSystem &inputSystem = InputSystem::instance();
	//ObjectSystem &objectSystem = ObjectSystem::instance();

	systems.push_back(&renderSystem);
	systems.push_back(&assetSystem);
	systems.push_back(&inputSystem);
	systems.push_back(&objectSystem);

	tickingSystems.push_back(&renderSystem);
	tickingSystems.push_back(&inputSystem);
	tickingSystems.push_back(&objectSystem);
}
Engine::~Engine(){
}
void Engine::initialize() {
	for (auto &s : systems)
		s->initialize();
}
void Engine::shutdown() {
	for (auto& s : systems)
		s->shutdown();
}
void Engine::onPlay() {
	for (auto& s : systems)
		s->onPlay();
}
void Engine::onEndplay() {
	for (auto& s : systems)
		s->onEndplay();
}
void Engine::tick() {
	for (auto &s : tickingSystems)
		s->tick();
}