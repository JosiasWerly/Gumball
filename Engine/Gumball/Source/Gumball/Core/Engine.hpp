#pragma once
#ifndef _engine_
#define _engine_
#include <list>

#include <Gumball/Singleton.hpp>
#include <Gumball/Containers/Codex.hpp>
#include <Gumball/Flow/StateMachine.hpp>
#include <Gumball/Concurrent/Scheduler.hpp>

class ProjectTarget;
namespace Plugin {
	class Controller;
};

namespace Engine {

class GENGINE Core : public Singleton<Core> {
	friend int ::main(int argc, char *argv[]);
	struct Init {
		int argc;
		char **argv;
		const char *engineDir;
		Concurrent::Scheduler *scheduler;
		void (*fnInjectModules)(Plugin::Controller *mCtrl);
	};

	Plugin::Controller *pluginCtrl;
	ProjectTarget *projectTarget;
	Flow::StateMachine::StateMachine fsm;
	
	Core();
	~Core();
	void Initialize(Init init);
	void Tick();

public:
	enum class eState { idle, play, hotreload, exit };

	void signal(eState signal) { fsm.to(signal); }
};

struct GENGINE CoreCodex : public Containers::Codex {
	friend class Core;
	CoreCodex() = default;
};

};
#endif // !__engine