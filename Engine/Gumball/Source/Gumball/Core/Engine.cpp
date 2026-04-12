#include "Engine.hpp"

#include <Plugin/Controller.hpp>
#include <Resource/Controller.hpp>
#include "Domain.hpp"
#include <iostream>
#include <string>

using namespace std;
namespace Engine {

Core::Core() {
	scheduler = new Concurrent::Scheduler;
	resourceCtrl = new Resource::Controller;
	pluginCtrl = new Plugin::Controller;
	project = new Plugin::ProjectLinker;
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
		init.scheduler = scheduler;
	}

	{//add resource
		CoreCodex::Add<Resource::Controller>(resourceCtrl);
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
			pluginCtrl->Hotreload();
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