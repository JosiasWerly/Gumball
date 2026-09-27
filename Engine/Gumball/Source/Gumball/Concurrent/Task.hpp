#pragma once
#ifndef __task
#define __task

#include <Gumball/Containers/Dispatcher.hpp>
#include <Gumball/Containers/Pointer.hpp>

#include "Common.hpp"

int main(int argc, char *argv[]);

namespace Concurrent {
using namespace std;

class ATask {
	friend class Task;
	friend class Scheduler;
	using FRun = Signal<void(ATask &)>;
	using FEnd = Signal<void(const ATask &)>;
	enum class eState : char { Idle, Scheduled, Done };
	enum class eResult : char { Continue, Success, Failed };

	Atomic<eState> state{ eState::Idle };
	eResult result;
	FRun run;
	FEnd end;
	void *data = nullptr;

public:
	inline void *Data() { return data; }
	inline void Complete(bool success) { result = success ? eResult::Success : eResult::Failed; }
	inline bool IsSuccess() const { return result == eResult::Success; }
	inline bool IsCompleted() const { return result != eResult::Continue; }
};

class Task {
	Ptr<ATask> htask;

public:
	Task();
	Task(ATask::FRun frun, ATask::FEnd fend, void *data);
	void Bind(ATask::FRun frun, ATask::FEnd fend, void *data = nullptr);
	void Start();
	void Stop();

	ATask::FRun &Run() { return htask->run; }
	ATask::FEnd &End() { return htask->end; }
	inline void *Data() { return htask->Data(); }
	inline void Complete(bool success) { htask->Complete(success); }
	inline bool IsSuccess() const { return htask->IsSuccess(); }
	inline bool IsCompleted() const { return htask->IsCompleted(); }

	bool operator==(const ATask &h) const { return (void*)htask == &h; }
};

struct TaskPool {
	Mutex mrequest;
	std::list<Ptr<ATask>> requests;
	std::list<Ptr<ATask>>::iterator iterator;
	AsyncBuffer<ATask *, 16> queue;
	TaskPool() : iterator(requests.begin()) {}
};

};
#endif // __task