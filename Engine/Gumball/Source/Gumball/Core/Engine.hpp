#pragma once
#ifndef _engine_
#define _engine_
#include <list>

#include <Gumball/Containers/Singleton.hpp>
#include <Gumball/Containers/Codex.hpp>
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

	Core();
	~Core();
	void Initialize(Init init);

public:
	Containers::Codex codex;	
	
	template<class T> 
	using Global = Global<T, []()->T & { return Core::Instance().codex.Get<T>(); }>;
};

};
#endif // !__engine