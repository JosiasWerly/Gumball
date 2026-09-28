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
	*htask->data = data;
}
void Task::Start() {
	htask->state.store(ATask::eState::Waiting, std::memory_order_relaxed);
	Core::Instance().codex.Get<Scheduler>().Add(htask);
}
void Task::Stop() {
	Core::Instance().codex.Get<Scheduler>().Pop(htask);
	htask->state.store(ATask::eState::Idle, std::memory_order_relaxed);
}
bool Task::Began() const {
	const ATask::eState st = htask->state.load(std::memory_order_relaxed);
	return st == ATask::eState::Waiting || st == ATask::eState::Scheduled;
}

void TaskSequence::OnTaskCompleted(const ATask &) {
	tasks.pop_front();
	if (tasks.empty()) {
		End().Invoke(*htask);
	}
	else {
		Task &tsk = tasks.front();
		tsk.End().Bind({ this, &TaskSequence::OnTaskCompleted });
	}
}
void TaskSequence::Start() {
	Task &tsk = tasks.front();
	tsk.End().Bind({ this, &TaskSequence::OnTaskCompleted });
}
