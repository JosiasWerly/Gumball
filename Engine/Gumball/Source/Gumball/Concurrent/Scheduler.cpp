#include "Scheduler.hpp"

using namespace Concurrent;

Task &Job::add(const string name) {
	return tasks.emplace_back(Task(name));
}
void Job::pop(const string name) {
	for (list<Task>::iterator it = tasks.begin(); it != tasks.end(); ++it) {
		if ((*it) == name) {
			tasks.erase(it);
			return;
		}
	}
}
void Job::clear() {
	tasks.clear();
}


Work::Work(Work &o) {
	state = o.state;
	job = o.job;
	tasksCompleted = o.tasksCompleted;
}
void Work::markStart() {
	state = eState::schedule;
	tasksMask = 0;
	tasksCompleted = 0;
	job->begin();
}
void Work::markTake() {
	taken = true;
}
void Work::markRelease() {
	taken = false;
}
void Work::markCompleted() {
	taken = false;
	state = eState::idle;
	job->end();
}


void Scheduler::run() {
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

			if (!work->isValid()) {
				workPool.pop(*work);
				work = nullptr;
				continue;
			}

			if (work->canStart()) {
				work->markStart();
				work->markTake();
			}
			else if (work->canTake()) {
				work->markTake();
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
					work->markCompleted();
				}
				else {
					work->markRelease();
				}
			}
		}
	}
}
void Scheduler::add(Job &job) {
	Work &work = workPool.add();
	work.job = &job;
}
void Scheduler::pop(Job &job) {
	Work *work = nullptr;
	for (auto &w : workPool.pool) {
		if (w.job == &job) {
			work = &w;
			work->job = nullptr;
			break;
		}
	}
}
void Scheduler::start(unsigned threadCount) {
	active = true;
	for (size_t i = 0; i < threadCount; i++)
		threads.emplace_back(jthread(&Scheduler::run, this));
}
void Scheduler::stop() {
	active = false;
}