#pragma once
#ifndef __engine
#define __engine
#include <list>

#include "Singleton.hpp"
#include "Containers/Codex.hpp"

class ModuleController;
class ProjectTarget;

struct EngineInit {
	int argc;
	char **argv;
	const char *engineDir;
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
	Codex codex;

	Engine();
	~Engine();
	void initialize(EngineInit data);
	void tick();

public:
	ModuleController *getModuleController() { return moduleController; }
	State &getState() { return state; }
	Codex &getCodex() { return codex; }
	
};

template<class T> class EngineSingleton {
protected:
	EngineSingleton() = default;
	virtual ~EngineSingleton() = default;

public:
	static T *instance() {
		static T *inst = Engine::instance()->getCodex().get<T>();
		return inst;
	}
};

#endif // !__engine