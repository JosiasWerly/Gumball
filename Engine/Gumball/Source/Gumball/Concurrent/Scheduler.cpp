#include "Scheduler.hpp"
#include <string>
using namespace Concurrent;

Work::Work(Work &o) {
	state = o.state;
	job = o.job;
}

Task &Job::Add(const char *name) {
	return taskCache.contains(name) ?
		*taskCache[name] :
		*(taskCache[name] = &tasks.emplace_back(name));
}
void Job::Pop(const char *name) {
	if (taskCache.contains(name)) {
		tasks.remove(*taskCache[name]);
		taskCache.erase(name);
	}
}
Task *Job::At(const char *name) {
	return taskCache.contains(name) ?
		taskCache[name] :
		nullptr;
}

void Scheduler::Run() {
	using eState = Work::eState;
	while (true) {
		{
			GuardUnique lock(workPool.m);
			cvjobs.wait(lock, [this]() { return !active || !workPool.isEmpty(); });
			if (!active)
				return;
		}

		Work *work = nullptr;
		{
			{
				GuardUnique lock(workPool.m);
				work = workPool.next();
				if (!work)
					continue;
			}

			if (!work->job) {
				workPool.pop(*work);
				work = nullptr;
				continue;
			}
			else if (work->Is(eState::start)) {
				work->Set(eState::running);
				work->job->tasksMask = 0;
				work->job->tasksCompleted = 0;
				work->job->begin(work->job);
			}
			else if (work->Is(eState::stall)) {
				work->Set(eState::running);
			}
			else {
				work = nullptr;
				continue;
			}
		}

		if (work) {
			Job *job = work->job;
			list<Task> *tasks = &job->tasks;

			unsigned char bitNum = 0;
			for (list<Task>::iterator it = tasks->begin(); it != tasks->end(); ++it, ++bitNum) {
				const bool completed = job->tasksMask & (1 << bitNum);
				if (!completed && (*it).fn()) {
					job->tasksMask |= 1 << bitNum;
					job->tasksCompleted++;
				}
			}

			{
				GuardUnique lock(workPool.m);
				if (job->tasksCompleted == tasks->size()) {
					work->state = eState::start;
					job->end(job);
				}
				else {
					work->Set(eState::stall);
				}
			}
		}
	}
}
void Scheduler::Add(Job &job) {
	GuardUnique lock(workPool.m);
	Work &w = workPool.add();
	w.job = &job;
	w.state = Work::eState::start;
}
void Scheduler::Pop(Job &job) {
	GuardUnique lock(workPool.m);
	Work *work = nullptr;
	{
		for (auto &w : workPool.pool) {
			if (w.job == &job) {
				work = &w;
				break;
			}
		}
	}
	if (!work)
		return;
	work->job = nullptr;
}
void Scheduler::Initialize(unsigned threadCount) {
	active = true;
	for (size_t i = 0; i < threadCount; i++)
		threads.emplace_back(jthread(&Scheduler::Run, this));
}
void Scheduler::Shutdown() {
	active = false;
}