#include "Engine.hpp"

#include <Concurrent/Scheduler.hpp>
#include <Plugin/Controller.hpp>
#include <Resource/Controller.hpp>
#include "Domain.hpp"

#include <iostream>
#include <string>

using namespace std;
namespace Core {

Engine::Engine() {
	scheduler = new Concurrent::Scheduler;
	resourceCtrl = new Resource::Controller;
	pluginCtrl = new Plugin::Controller;
	project = new Plugin::ProjectLinker;
}
Engine::~Engine() {
}
void Engine::Initialize(Init init) {
	{
		init.fnInjectModules(pluginCtrl);
		codex.Add<Plugin::Controller>(pluginCtrl);
	}
	
	{//add domain		
		Domain &domain = codex.Add<Domain>();
		domain.applicationPath = init.argv[0];
		domain.applicationDir = domain.applicationPath.substr(0, domain.applicationPath.find_last_of("\\")) + "\\";
		domain.engineDir = init.engineDir;
		domain.contentPath = domain.engineDir + "Content\\";
	}
	
	{//add scheduler
		codex.Add<Concurrent::Scheduler>(scheduler);
		init.scheduler = scheduler;
	}

	{//add resource
		codex.Add<Resource::Controller>(resourceCtrl);
	}
	
	pluginCtrl->State(Plugin::Controller::eState::startup);} //start the whole thing

};

