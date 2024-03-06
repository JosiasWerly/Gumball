#include "SystemController.hpp"

void System::initialize() {}
void System::lateInitialize() {}
void System::shutdown() {}
void System::beginPlay() {}
void System::endPlay() {}
void System::tick(const double &deltaTime) {}
ESystemTickType System::tickType() { return ESystemTickType::none; }

SystemController::SystemController() {
}
void SystemController::initialize() const {
	for (auto &s : systems)
		s->initialize();
}
void SystemController::lateInitialize() const {
	for (auto &s : systems)
		s->lateInitialize();
}
void SystemController::shutdown() const {
	for (auto &s : systems)
		s->shutdown();
}
void SystemController::beginPlay() const {
	for (auto &s : systems)
		s->beginPlay();
}
void SystemController::endPlay() const {
	for (auto &s : systems)
		s->endPlay();
}