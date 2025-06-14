#include "Engine.hpp"

#include "Module.hpp"
#include "ProjectTarget.hpp"
#include "Domain.hpp"

#include <iostream>
#include <string>

using namespace std;


#include "Flow/Dispatcher.hpp"
#include "Flow/StateMachine.hpp"

struct A {
	void foo(int a) { cout << "A:" << a << endl; }
};
void test() {
	using namespace FSM;
	enum class eState {
		Root, Sum, Sub,
		Calculate
	};
	StateMachine sm;
	sm.push(eState::Root);
	sm.push(eState::Sum);
	sm.push(eState::Sub);
	sm.push(eState::Calculate);
	int valA, valB;

	sm[eState::Root].onEnter += [&](Controller &c) {
		cout << "ROOT:\n0. sub\n1. sum" << endl;
		int id = 0;
		cin >> id;
		cout << endl;
		c = id ? eState::Sum : eState::Sub;
	};
	sm[eState::Sum].onEnterFrom[eState::Root] += [&](Controller &c) {
		cout << "SUM: from ROOT" << endl;
		cout << "enter valA and valB" << endl;
		cin >> valA;
		cin >> valB;
		cout << endl;
		c = eState::Calculate;
	};
	sm[eState::Sub].onEnterFrom[eState::Root] += [&](Controller &c) {
		cout << "SUB: from ROOT" << endl;
		cout << "enter valA and valB" << endl;
		cin >> valA;
		cin >> valB;
		cout << endl;
		c = eState::Calculate;
	};
	{
		auto &s = sm[eState::Calculate];
		s.onEnter += [&](Controller &c) {
			c = eState::Root;
		};
		s.onEnterFrom[eState::Sum] += [&](Controller &c) {
			cout << "CALCULATE: from SUM" << endl;
			cout << "res:" << valA + valB << endl;
			cout << endl;
		};
		s.onEnterFrom[eState::Sub] += [&](Controller &c) {
			cout << "CALCULATE: from SUB" << endl;
			cout << "res:" << valA - valB << endl;
			cout << endl;
		};
	}


	A a;
	Event<void(int a)> aa;
	aa += [](int a) { cout << "fnx" << a << endl; };
	aa += [&](int a) { cout << "ref" << a << endl; };
	aa += [=](int a) { cout << "cpy" << a << endl; };
	aa.operator+=<A>({ &a, &A::foo });
	aa(2);
	while (1) { sm.run(); }
}

Engine::Engine() {
	moduleController = new ModuleController;
	//Activator::setInstance(new Activator);
	////Activator::instance()->addPackage<ClassTypePackage_Core>();

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
	test();
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