#include "Engine.hpp"

#include "Plugin/Controller.hpp"
#include "ProjectTarget.hpp"
#include "Domain.hpp"


#include <iostream>
#include <string>

using namespace std;
namespace Engine {

Core::Core() {
	pluginCtrl = new Plugin::Controller;
	projectTarget = new ProjectTarget;
}
Core::~Core() {
}
void Core::Initialize(Init init) {
	init.fnInjectModules(pluginCtrl);
	
	{//add domain
		Domain *domain = CoreCodex::Add<Domain>();
		domain->applicationPath = init.argv[0];
		domain->applicationDir = domain->applicationPath.substr(0, domain->applicationPath.find_last_of("\\")) + "\\";
		domain->engineDir = init.engineDir;
		domain->contentPath = domain->engineDir + "Content\\";
	}
	
	{//add scheduler
		CoreCodex::Add<Concurrent::Scheduler>(init.scheduler);
	}
	
	{//bind states
		fsm[eState::play].onEnter.bind([&]() {
			pluginCtrl->BeginPlay();
		});
		fsm[eState::play].onExit.bind([&]() {
			pluginCtrl->EndPlay();
		});
		fsm[eState::play].onTick.bind([&]() {
			const double deltaTime = 0.1;
			pluginCtrl->Tick<Plugin::eTick::gameplay>(deltaTime);
		});
		fsm[eState::hotreload].onEnter.bind([&]() {
			pluginCtrl->BeginHotReload();
			projectTarget->unload();
			projectTarget->load();
			pluginCtrl->EndHotReload();
			fsm.to(eState::idle);
		});
	}
	pluginCtrl->Startup();
}
void Core::Tick() {
	while (true);
	//moduleController->Startup();
	//fsm.set(eState::hotreload);
	//fsm.tick();
	//scheduler.Start(1);
	//while (fsm.now() != eState::exit) {
	//	moduleController->Tick<EModuleTickType::editor>(deltaTime);
	//	fsm.tick();
	//}
	//moduleController->Shutdown();
}

};