#include "Engine.hpp"

#include "Module.hpp"
#include "ProjectTarget.hpp"
#include "Domain.hpp"

#include <iostream>
#include <string>

using namespace std;

Engine::Engine() {
	moduleController = new ModuleController;
	projectTarget = new ProjectTarget;
}
Engine::~Engine() {
}
void Engine::initialize(EngineInit data) {
	data.fnInjectModules(moduleController);
	Domain *domain = new Domain;
	domain->applicationPath = data.argv[0];
	domain->applicationDir = domain->applicationPath.substr(0, domain->applicationPath.find_last_of("\\")) + "\\";
	domain->engineDir = data.engineDir;
	domain->contentPath = domain->engineDir + "Content\\";
	codex.add<Domain>(domain);
}
void Engine::tick() {
	moduleController->load();
	state.loadState = State::ELoadState::load;
	state.playState = State::EPlayState::beginPlay;
	
	while (true) {
		const double deltaTime = 0.1;
		
		switch (state.loadState) {
			case State::ELoadState::load: {
				projectTarget->load();
				moduleController->startup();
				state.loadState = State::ELoadState::none;
				break;
			}
			case State::ELoadState::unload: {
				moduleController->shutdown();
				projectTarget->unload();
				state.loadState = State::ELoadState::load;
				break;
			}
		}

		moduleController->tick<EModuleTickType::editor>(deltaTime);

		switch (state.playState) {
			case State::EPlayState::beginPlay: {
				moduleController->beginPlay();
				state.playState = State::EPlayState::playing;
				break;
			}
			case State::EPlayState::playing: {
				moduleController->tick<EModuleTickType::gameplay>(deltaTime);
				break;
			}
			case State::EPlayState::endPlay: {
				moduleController->endPlay();
				state.playState = State::EPlayState::disabled;
				break;
			}
		}

		if (state.hasSignal) {
			switch (state.signal) {
				case State::ESignal::stop: {
					if (state.playState != State::EPlayState::disabled) {
						state.playState = State::EPlayState::disabled;
					}
					else {
						state.loadState = State::ELoadState::unload;
						state.hasSignal = false;
					}
					break;
				}
				case State::ESignal::play: {
					const bool canTransition =
						state.loadState == State::ELoadState::none &&
						state.playState == State::EPlayState::disabled;
					if (canTransition) {
						state.playState = State::EPlayState::playing;
						state.hasSignal = false;
					}
					break;
				}
				case State::ESignal::pause: {
					const bool canTransition =
						state.loadState == State::ELoadState::none &&
						state.playState == State::EPlayState::playing;
					if (canTransition) {
						state.playState = State::EPlayState::disabled;
						state.hasSignal = false;
					}
					break;
				}
			}
		}
	}

	moduleController->endPlay();
	moduleController->shutdown();
	moduleController->unload();
}