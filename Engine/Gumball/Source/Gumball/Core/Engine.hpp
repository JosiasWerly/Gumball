#pragma once
#ifndef __engine
#define __engine
#include <Gumball/Containers/Singleton.hpp>
#include <Gumball/Containers/Codex.hpp>

#include <list>

int main(int argc, char *argv[]);
namespace Plugin {
	class Controller;
	class ProjectLinker;
};

namespace Resource {
	class Controller;
};

namespace Concurrent {
	class Scheduler;
};

namespace Core {

class GENGINE Engine : public Singleton<Engine> {
	friend int ::main(int argc, char *argv[]);
	template<class T> friend class Global;
	struct Init {
		int argc;
		char **argv;
		const char *engineDir;
		Concurrent::Scheduler *&scheduler;
		void (*fnInjectModules)(Plugin::Controller *mCtrl);
	};
	
	Containers::Codex codex;
	Concurrent::Scheduler *scheduler;
	Resource::Controller *resourceCtrl;
	Plugin::Controller *pluginCtrl;
	Plugin::ProjectLinker *project;

	Engine();
	~Engine();
	void Initialize(Init init);

public:
	Containers::Codex &Codex() { return codex; }
};

template<class T>
class Global {
protected:
	Global() = default;
	virtual ~Global() = default;
	Global(Global &r) = delete;
	void operator=(Global &r) = delete;

public:
	static T &Instance() {
		static T *inst = &Engine::Instance().Codex().Get<T>();
		return *inst;
	}
};

	
	
};
#endif // !__engine