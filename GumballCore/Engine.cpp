#include "Engine.hpp"
#include "AssetManager.hpp"
#include "RenderSystem.hpp"

#include <string>
using namespace std;

template<class T> string getClassName(){
	string out = typeid(T).name();
	return out.substr(6, out.size() - 6);
}
//Systems.emplace(getClassName<AssetsSystem>(), &AssetsSystem::instance());



Engine::Engine() {
	Systems.push_back(&AssetsSystem::instance());
	Systems.push_back(&RenderSystem::instance());
	Systems.push_back(&InputSystem::instance());

	TickingSystems.push_back(&RenderSystem::instance());
	TickingSystems.push_back(&InputSystem::instance());
}
Engine::~Engine(){
}
void Engine::initialize() {
	for (auto &s : Systems)
		s->initialize();
}
void Engine::shutdown() {
	for (auto& s : Systems)
		s->shutdown();
}
void Engine::onPlay() {
	for (auto& s : Systems)
		s->onPlay();
}
void Engine::onEndplay() {
	for (auto& s : Systems)
		s->onEndplay();
}
void Engine::tick() {
	initialize();	
	onPlay();
	while (true) {
		for (auto& s : TickingSystems)
			s->tick();
	}
	onEndplay();
	shutdown();
}