#pragma once
#ifndef _engine
#define _engine
#include <list>

#include "Patterns.hpp"


class ModuleController;
class ProjectTarget;

struct EngineInit {
	int argc;
	char **argv;
	void (*fnInjectModules)(ModuleController *mCtrl);
};




class GENGINE Engine : public Singleton<Engine> {
public:
	struct GENGINE State {
		friend class Engine;

	public:
		enum class EPlayState {
			disabled, beginPlay, playing, endPlay,
		};
		enum class ELoadState {
			none, load, unload
		};

		enum class ESignal {
			stop, play, pause
		};
	private:
		EPlayState playState = EPlayState::disabled;
		ELoadState loadState = ELoadState::none;

		bool hasSignal = false;
		ESignal signal = ESignal::stop;

	public:
		Inline EPlayState getPlayState() const { return playState; }
		Inline ELoadState getLoadState() const { return loadState; }

		void newSignal(ESignal newSignal) { signal = newSignal; hasSignal = true; }
	};

private:
	friend int main(int argc, char *argv[]);
	ModuleController *moduleController;
	ProjectTarget *projectTarget;
	State state;

	Engine();
	~Engine();
	void initialize(EngineInit data);
	void tick();

public:
	ModuleController *getModuleController() { return moduleController; }
	State &getState() { return state; }
	
};
#endif // !_engine