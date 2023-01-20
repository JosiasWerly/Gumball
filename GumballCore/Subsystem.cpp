#include "Subsystem.hpp"


Subsystem::Subsystem() {}
Subsystem::~Subsystem() {}
void Subsystem::initialize() {}
void Subsystem::shutdown() {}
void Subsystem::beginPlay() {}
void Subsystem::endPlay() {}
void Subsystem::tick(const double &deltaTime) {}
ESystemTickType Subsystem::tickMode() { return ESystemTickType::disable; }

SystemOverseer::SystemOverseer() {
}
Inline void SystemOverseer::beginPlay() const {
	for (auto &s : systems)
		s->beginPlay();
}
Inline void SystemOverseer::endPlay() const {
	for (auto &s : systems)
		s->endPlay();
}
Inline void SystemOverseer::shutdown() const {
	for (auto &s : systems)
		s->shutdown();
}
Inline void SystemOverseer::initialize() const {
	for (auto &s : systems)
		s->initialize();
}
