#pragma once
#ifndef __task
#define __task

#include <Gumball/Containers/Dispatcher.hpp>
#include <Gumball/Containers/Pointer.hpp>

#include "Common.hpp"

int main(int argc, char *argv[]);

namespace Concurrent {
using namespace std;

struct ATask {
	using FRun = Signal<bool(void *)>;
	using FEnd = Signal<void(void *)>;
	enum class eState : char { Idle, Scheduled, Done };

	Atomic<eState> state{ eState::Idle };
	FRun run;
	FEnd end;
	void *data = nullptr;

	inline bool Run() const { return run(data); }
	inline void End() const { if (end.isBound()) end(data); }
};

class GENGINE Task {
	Ptr<ATask> htask;

public:
	Task();
	Task(ATask::FRun frun, ATask::FEnd fend, void *data);
	void Bind(ATask::FRun frun, ATask::FEnd fend, void *data = nullptr);
	void Start();
	void Stop();

	ATask::FRun &Run() { return htask->run; }
	ATask::FEnd &End() { return htask->end; }
	void *&Data() { return htask->data; }
};
};
#endif // __task