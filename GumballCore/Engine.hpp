#pragma once
#ifndef _engine
#define _engine
#include <list>

#include "Patterns.hpp"
#include "TimeStat.hpp"
#include "Activator.hpp"

class SystemController;
class ProjectLinker;
class TimeStat;

class EditorSystem;
class RenderSystem;
class AssetsSystem;
class InputSystem;
class WorldSystem;

void test();

class GBCORE Engine : 
	public Singleton<Engine> {
private:
	friend int main(int, char *[]);

	TimeStat timeStats;
	ProjectLinker *projectLinker;	
	SystemController *systemController;
	
	enum class EPlayState {
		disabled, beginPlay, playing, endPlay,
	} playState = EPlayState::disabled;
	enum class ELoadState {
		none, load, unload
	} loadState = ELoadState::none;
	
	Engine();
	~Engine();	
public:
	void args(int argc, char *argv[]);
	void tick();

	void signalLoad() { loadState = ELoadState::load; }
	void signalUnload() { loadState = ELoadState::unload; }
	void signalPlay() { playState = EPlayState::beginPlay; }
	void signalPause() { playState = EPlayState::disabled; }
	void signalStop() { playState = EPlayState::endPlay; }
	

	//self explanatory names
	RenderSystem *renderSystem;
	AssetsSystem *assetSystem;
	InputSystem *inputSystem;
	WorldSystem *worldSystem;
	EditorSystem *editorSystem;
};
#endif // !_engine