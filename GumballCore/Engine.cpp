#include "Engine.hpp"
#include "AssetManager.hpp"
#include <string>
using namespace std;

template<class T>string getClassName(){
	string out = typeid(T).name();
	return out.substr(6, out.size() - 6);
}

Engine::Engine() {
	Systems.emplace(getClassName<AssetsSystem>(), new AssetsSystem);
}
Engine::~Engine(){

}
void Engine::Initialize() {
	for (auto &kv : Systems)
		kv.second->Initialize();
}
void Engine::Shutdown() {
	for (auto &kv : Systems)
		kv.second->Shutdown();
}
void Engine::OnPlay() {
	for (auto &kv : Systems)
		kv.second->OnPlay();
}
void Engine::OnEndplay() {
	for (auto &kv : Systems)
		kv.second->OnEndplay();
}


template<class T> T *Engine::getSystem() {
	string name = getClassName<T>();
	if (Systems.count(name))
		return reinterpret_cast<T*>(Systems[name]);
	return nullptr;
}