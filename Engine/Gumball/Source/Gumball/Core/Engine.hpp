#pragma once
#ifndef __engine
#define __engine
#include <Gumball/Containers/Singleton.hpp>
#include <Gumball/Containers/Codex.hpp>
#include <Gumball/Concurrent/Scheduler.hpp>

#include <list>

namespace Plugin {
	class Controller;
	class ProjectLinker;
};

namespace Resource {
	class Controller;
};

namespace Core {

class GENGINE Engine : public Singleton<Engine> {
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

	Engine();
	~Engine();
	void Initialize(Init init);

public:
	Containers::Codex codex;	
	
	template<class T> 
	using Global = Global<T, []()->T & { return Instance().codex.Get<T>(); }>;
};

};
#endif // !__engine