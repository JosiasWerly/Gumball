#include "Engine.hpp"
#include "AssetManager.hpp"
#include "RenderSystem.hpp"
#include "Object.hpp"

#include <string>
using namespace std;

template<class T> string getClassName(){
	string out = typeid(T).name();
	return out.substr(6, out.size() - 6);
}
//Systems.emplace(getClassName<AssetsSystem>(), &AssetsSystem::instance());



Engine::Engine() {
	systems.push_back(&AssetsSystem::instance());
	systems.push_back(&RenderSystem::instance());
	systems.push_back(&InputSystem::instance());
	systems.push_back(&ObjectSystem::instance());

	tickingSystems.push_back(&RenderSystem::instance());
	tickingSystems.push_back(&InputSystem::instance());
	tickingSystems.push_back(&ObjectSystem::instance());
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
	initialize();	
	onPlay();
	while (true) {
		for (auto& s : tickingSystems)
			s->tick();
	}
	onEndplay();
	shutdown();
}