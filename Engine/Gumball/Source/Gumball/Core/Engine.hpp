#pragma once
#ifndef _engine_
#define _engine_
#include <list>

#include <Gumball/Containers/Singleton.hpp>
#include <Gumball/Containers/Codex.hpp>
#include <Gumball/Flow/StateMachine.hpp>
#include <Gumball/Concurrent/Scheduler.hpp>

namespace Plugin {
	class Controller;
	class ProjectLinker;
};

namespace Resource {
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
	Resource::Controller *resourceCtrl;
	Plugin::Controller *pluginCtrl;
	Plugin::ProjectLinker *project;
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