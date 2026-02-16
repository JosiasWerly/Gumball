#pragma once
#ifndef _engine_
#define _engine_
#include <list>

#include "Singleton.hpp"
#include "Containers/Codex.hpp"
#include "Flow/StateMachine.hpp"
#include "Concurrent/Scheduler.hpp"

namespace Plugin {
	class Controller;
};
class ProjectTarget;

struct EngineInit {
	int argc;
	char **argv;
	const char *engineDir;
	Concurrent::Scheduler *scheduler;
	void (*fnInjectModules)(Plugin::Controller *mCtrl);
};

class GENGINE Engine : public Singleton<Engine> {
	friend int main(int argc, char *argv[]);
	
	Plugin::Controller *pluginCtrl;
	ProjectTarget *projectTarget;
	Codex codex;
	Flow::StateMachine::StateMachine fsm;
	Concurrent::Scheduler *scheduler;

	Engine();
	~Engine();
	void initialize(EngineInit data);
	void tick();

public:
	enum class eState { idle, play, hotreload, exit	};

	void signal(eState signal) { fsm.to(signal); }
	Codex &Codex() { return codex; }
	Plugin::Controller &PluginController() { return *pluginCtrl; }
	Concurrent::Scheduler &Scheduler() { return *scheduler; }
};

template<class T> class EngineSingleton {
protected:
	EngineSingleton() = default;
	virtual ~EngineSingleton() = default;

public:
	static T *instance() {
		static T *inst = Engine::instance()->Codex().Get<T>();
		return inst;
	}
};

#endif // !__engine