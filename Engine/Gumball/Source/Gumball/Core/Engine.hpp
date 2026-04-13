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
		Concurrent::Scheduler *&scheduler;
		void (*fnInjectModules)(Plugin::Controller *mCtrl);
	};
	Concurrent::Scheduler *scheduler;
	Resource::Controller *resourceCtrl;
	Plugin::Controller *pluginCtrl;
	Plugin::ProjectLinker *project;


	Concurrent::Job tick;
	Containers::TypeCodex codex;
	Flow::StateMachine::StateMachine fsm;
	
	Core();
	~Core();
	void Initialize(Init init);
	void Tick(Concurrent::Job *);

public:
	template<class T> using Global = Global<T, []()->T & { return Core::Instance().codex.Get<T>(); }>;
	enum class eState { idle, play, hotreload, exit };
	void signal(eState signal) { fsm.to(signal); }
};

};
#endif // !__engine