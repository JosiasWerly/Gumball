#include "Scheduler.hpp"
#include "Task.hpp"
using namespace Concurrent;

Scheduler::Scheduler() {
}
void Scheduler::Initialize(unsigned char thCount) {
	for (unsigned i = 0; i < thCount; ++i) {
		threads.emplace_back(std::jthread(&Scheduler::ConsumerTick, this));
	}
}
void Scheduler::Shutdown() {
	active.store(false, std::memory_order_relaxed);
	for (auto &th : threads) {
		th.join();
	}
}
void Scheduler::ProducerTick() {
	while (active.load(std::memory_order_relaxed)) {
		{//tasks
			Lock l(tasks.mrequest);
			if (tasks.iterator == tasks.requests.end())
				tasks.iterator = tasks.requests.begin();

			while (tasks.iterator != tasks.requests.end()) {
				ATask &tsk = *(*tasks.iterator);
				const ATask::eState st = tsk.state.load(std::memory_order_acquire);
				if (st == ATask::eState::Done) {
					tasks.iterator = tasks.requests.erase(tasks.iterator);
					continue;
				}
				else if (!tasks.queue.Full() && st == ATask::eState::Waiting) {
					if (tasks.queue.Push(&tsk)) {
						tsk.state.store(ATask::eState::Scheduled, std::memory_order_release);
						Lock cvl(mthread);
						cvthread.notify_one();
						++tasks.iterator;
					}
				}
			}
		}
		{//jobs
			Lock l(jobs.mrequest);
			if (jobs.iterator == jobs.requests.end())
				jobs.iterator = jobs.requests.begin();

			while (jobs.iterator != jobs.requests.end()) {
				AJob &jb = *(*jobs.iterator);
				const AJob::eState st = jb.state.load(std::memory_order_acquire);
				if (!jobs.queue.Full() && st == AJob::eState::Waiting) {
					if (jobs.queue.Push(&jb)) {
						jb.state.store(AJob::eState::Scheduled, std::memory_order_release);
						Lock cvl(mthread);
						cvthread.notify_one();
						++jobs.iterator;
					}
				}
			}
		}
	}
}
void Scheduler::ConsumerTick() {
	while (active.load(std::memory_order_relaxed)) {
		{
			ULock l(mthread);
			cvthread.wait(l, [&]()->bool {
				return
					!tasks.queue.Empty() ||
					!jobs.queue.Empty();
			});
		}

		for (ATask *tsk = nullptr; tasks.queue.Pop(tsk); tsk = nullptr) {
			using eTaskState = ATask::eState;
			using eTaskResult = ATask::eResult;

			tsk->run(*tsk);
			const eTaskResult res = tsk->result;
			if (res != eTaskResult::Continue) {
				tsk->state.store(eTaskState::Done, std::memory_order_release);
				if (tsk->end)
					tsk->end(*tsk);
			}
			else {
				tsk->state.store(eTaskState::Waiting, std::memory_order_release);
			}
		}

		for (AJob *jb = nullptr; jobs.queue.Pop(jb); jb = nullptr) {
			using eJobState = AJob::eState;
			jb->Run();
			jb->state.store(eJobState::Waiting, std::memory_order_release);
		}
	}
}
void Scheduler::Add(Ptr<ATask> &task) {
	Lock l(tasks.mrequest);
	tasks.requests.push_back(task);
}
void Scheduler::Pop(Ptr<ATask> &task) {
	Lock l(tasks.mrequest);
	auto it = std::find(tasks.requests.begin(), tasks.requests.end(), task);
	if (tasks.iterator == it)
		tasks.iterator = tasks.requests.erase(it);
}
void Scheduler::Add(Ptr<AJob> &job) {
	Lock l(jobs.mrequest);
	jobs.requests.push_back(job);
}
void Scheduler::Pop(Ptr<AJob> &job) {
	Lock l(jobs.mrequest);
	auto it = std::find(jobs.requests.begin(), jobs.requests.end(), job);
	jobs.requests.remove(job);
	if (jobs.iterator == it)
		jobs.iterator = jobs.requests.erase(it);
}