#include "Scheduler.hpp"
#include <string>
using namespace Concurrent;

Task *Job::Add() {
	return &tasks.emplace_back(Task());
}
void Job::Pop(Task *task) {
	tasks.remove(*task);
}
void Job::Clear() {
	tasks.clear();
}

Work::Work(Work &o) {
	state = o.state;
	job = o.job;
	tasksCompleted = o.tasksCompleted;
}
void Work::MarkStart() {
	state = eState::schedule;
	tasksMask = 0;
	tasksCompleted = 0;
	job->begin(job);
}
void Work::MarkTake() {
	taken = true;
}
void Work::MarkRelease() {
	taken = false;
}
void Work::MarkCompleted() {
	taken = false;
	state = eState::idle;
	job->end(job);
}

void Scheduler::Run() {
	while (true) {
		{
			GuardUnique lock(workPool.m);
			cvjobs.wait(lock, [this]() { return !active || !workPool.isEmpty(); });
			if (!active)
				return;
		}

		Work *work = nullptr;
		{
			GuardUnique lock(workPool.m);
			work = workPool.next();
			if (!work)
				continue;

			if (!work->IsValid()) {
				workPool.pop(*work);
				work = nullptr;
				continue;
			}

			if (work->CanStart()) {
				work->MarkStart();
				work->MarkTake();
			}
			else if (work->CanTake()) {
				work->MarkTake();
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
				const bool completed = work->tasksMask & (1 << bitNum);
				if (!completed && (*it).fn()) {
					work->tasksMask |= 1 << bitNum;
					work->tasksCompleted++;
				}
			}

			{
				GuardUnique lock(workPool.m);
				if (work->tasksCompleted == tasks->size()) {
					work->MarkCompleted();
				}
				else {
					work->MarkRelease();
				}
			}
		}
	}
}
Job* Scheduler::Add() {
	Work &work = workPool.add();
	work.job = &jobPool.add();
	return work.job;
}
void Scheduler::Pop(Job *job) {
	Work *work = nullptr;
	for (auto &w : workPool.pool) {
		if (w.job == job) {
			work = &w;
			work->job = nullptr;
			break;
		}
	}
	jobPool.pop(*job);
}
void Scheduler::Start(unsigned threadCount) {
	active = true;
	for (size_t i = 0; i < threadCount; i++)
		threads.emplace_back(jthread(&Scheduler::Run, this));
}
void Scheduler::Stop() {
	active = false;
}