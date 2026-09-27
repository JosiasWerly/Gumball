#include "Task.hpp"
#include "Scheduler.hpp"
#include <Gumball/Core/Engine.hpp>

using namespace Concurrent;
using namespace Engine;

Task::Task() : htask(new ATask) {}
Task::Task(ATask::FRun frun, ATask::FEnd fend, void *data) : 
	htask(new ATask) {
	Bind(frun, fend, data);
}
void Task::Bind(ATask::FRun frun, ATask::FEnd fend, void *data) {
	htask->run = frun;
	htask->end = fend;
	htask->data = data;
}
void Task::Start() {
	htask->state.store(ATask::eState::Idle, std::memory_order_relaxed);
	Core::Instance().codex.Get<Scheduler>().Add(htask);
}
void Task::Stop() {
	Core::Instance().codex.Get<Scheduler>().Pop(htask);
}