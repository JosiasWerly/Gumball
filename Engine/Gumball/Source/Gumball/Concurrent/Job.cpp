#include "Job.hpp"
#include "Scheduler.hpp"
#include <Gumball/Core/Engine.hpp>

using namespace Concurrent;
using namespace Core;



Job::Job() : hjob(new AJob) {}
void Job::Start() {
	hjob->state.store(AJob::eState::Waiting, std::memory_order_relaxed);
	Engine::Instance().codex.Get<Scheduler>().Add(hjob);
}
void Job::Stop() {
	Engine::Instance().codex.Get<Scheduler>().Pop(hjob);
	hjob->state.store(AJob::eState::Idle, std::memory_order_relaxed);
}
bool Job::Began() const {
	const AJob::eState st = hjob->state.load(std::memory_order_relaxed);
	return st == AJob::eState::Waiting || st == AJob::eState::Scheduled;
}
