#include "Scheduler.hpp"
#include "Task.hpp"
using namespace Concurrent;

Scheduler::Scheduler() {
}
void Scheduler::Initialize(unsigned char thCount) {
	for (unsigned i = 0; i < thCount; ++i)
		threads.emplace_back(std::jthread(&Scheduler::ConsumerTick, this));
}
void Scheduler::Shutdown() {}
void Scheduler::Tick() {
	while (true) {
		{//tasks
			Lock l(tasks.mrequest);
			for (auto it = tasks.requests.begin(); it != tasks.requests.end();) {
				ATask &tsk = *(*it);
				const ATask::eState st = tsk.state.load(std::memory_order_acquire);
				if (!tasks.queue.Full() && st == ATask::eState::Idle) {
					tsk.state.store(ATask::eState::Scheduled);
					tasks.queue.Push(&tsk);
					cvthread.notify_one();
				}
				else if (st == ATask::eState::Done) {
					it = tasks.requests.erase(it);
					continue;
				}
				++it;
			}
		}
	}
}
void Scheduler::ConsumerTick() {
	while (true) {
		{
			ULock l(mthread);
			cvthread.wait(l, [&]()->bool { return !tasks.queue.Empty(); });
		}

		while (ATask *tsk = tasks.queue.Pop()) {
			using eState = ATask::eState;
			if (tsk->Run()) {
				tsk->state.store(eState::Done);
				tsk->End();
			}
			else
				tsk->state.store(eState::Idle);
		}
	}
}
void Scheduler::Add(Ptr<ATask> &task) {
	Lock l(tasks.mrequest);
	task->state.store(ATask::eState::Idle);
	tasks.requests.push_back(task);
}
void Scheduler::Pop(Ptr<ATask> &task) {
	Lock l(tasks.mrequest);
	tasks.requests.remove(task);
}