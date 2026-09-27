#include "Job.hpp"
#include "Scheduler.hpp"
#include <Gumball/Core/Engine.hpp>

using namespace Concurrent;
using namespace Engine;


void Job::Start() {
	hjob->state.store(AJob::eState::Idle, std::memory_order_relaxed);
	Core::Instance().codex.Get<Scheduler>().Add(hjob);
}
void Job::Stop() {
	Core::Instance().codex.Get<Scheduler>().Pop(hjob);
}
