#pragma once
#ifndef __engine
#define __engine
#include <list>

#include "Singleton.hpp"
#include "Containers/Codex.hpp"
#include "Flow/StateMachine.hpp"
#include "Flow/ChainMachine.hpp"
#include "Concurrent/Scheduler.hpp"

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
	enum class eSignal {
		idle, play, hotreload, exit
	};

private:
	friend int main(int argc, char *argv[]);
	ModuleController *moduleController;
	ProjectTarget *projectTarget;
	Codex codex;
	Flow::StateMachine::StateMachine fsm;

	Engine();
	~Engine();
	void initialize(EngineInit data);
	void tick();

public:
	ModuleController *getModuleController() { return moduleController; }
	void signal(eSignal signal) {
		fsm.to(signal);
	}
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