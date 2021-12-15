#include "Engine.hpp"
#include "AssetManager.hpp"

#include <string>
using namespace std;

template<class T>string getClassName(){
	string out = typeid(T).name();
	return out.substr(6, out.size() - 6);
}

Engine::Engine() {
	Systems.emplace(getClassName<AssetsSystem>(), &AssetsSystem::instance());
}
Engine::~Engine(){

}
void Engine::initialize() {
	for (auto &kv : Systems)
		kv.second->initialize();
}
void Engine::shutdown() {
	for (auto &kv : Systems)
		kv.second->shutdown();
}
void Engine::onPlay() {
	for (auto &kv : Systems)
		kv.second->onPlay();
}
void Engine::onEndplay() {
	for (auto &kv : Systems)
		kv.second->onEndplay();
}