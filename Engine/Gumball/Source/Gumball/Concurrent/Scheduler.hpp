#pragma once
#ifndef __scheduler
#define __scheduler

#include <thread>

#include <Gumball/Flow/Dispatcher.hpp>
#include <Gumball/Containers/Pointer.hpp>
#include "Common.hpp"

int main(int argc, char *argv[]);

namespace Concurrent {
using namespace std;

class GENGINE Task {
	friend class Scheduler;
	using Delegate = Signal<bool()>;
	
	const char *name;
	Delegate fn;

public:
	Task(const char *name) : name(name) {}
	bool operator==(const Task &other) const { return this == &other; }
	
	Delegate &Fn() { return fn; }
	const char *Name() const { return name; }
};

class GENGINE Job {
	friend class Scheduler;
	using Delegate = Signal<void(Job *)>;

	Mutex m;
	PtrWeak data;
	Delegate begin, end;
	std::list<Task> tasks;
	std::unordered_map<const char *, Task *> taskCache;
	unsigned tasksMask = 0;
	unsigned char tasksCompleted = 0;

public:
	bool operator==(const Job &other) const { return this == &other; }

	Task &Add(const char *name);
	void Pop(const char *name);
	Task *At(const char *name);
	PtrWeak &Data() { return data; }
	Delegate &Begin() { return begin; }
	Delegate &End() { return end; }
	bool HasConcluded() const { return tasksCompleted == tasks.size(); }
};

struct GENGINE Work {
	enum class eState : char { idle, start, running, stall, end };

	eState state = eState::idle;
	Job *job = nullptr;

	Work() = default;
	Work(Work &o);
	void Set(eState st) { state = st; }
	bool Is(eState st) const { return state == st; }
	bool operator==(const Work &o) const { return this == &o; }
};

class GENGINE Scheduler {
	friend int ::main(int argc, char *argv[]);

	std::atomic<bool> active;
	vector<jthread> threads;

	std::condition_variable cvjobs;
	SharedList<Work> workPool;

	void Run();

public:
	Scheduler() = default;
	void Add(Job &job);
	void Pop(Job &job);

	void Initialize(unsigned threadCount);
	void Shutdown();
};

};
#endif // __scheduler