#pragma once
#ifndef __job
#define __job

#include <Gumball/Containers/Dispatcher.hpp>
#include <Gumball/Containers/Pointer.hpp>

#include "Common.hpp"

int main(int argc, char *argv[]);

namespace Concurrent {
using namespace std;

class AJob {
	friend class Job;
	friend class Scheduler;
	using FRun = Signal<void(AJob &)>;
	enum class eState : char { Idle, Scheduled, Done };

	Atomic<eState> state{ eState::Idle };

	FRun run;
	void *data = nullptr;
	inline void Run() { run(*this); }
};

class Job {
	Ptr<AJob> hjob;

public:
	void Add(Job &other);
	void Pop(Job &other);
	void Start();
	void Stop();
};

struct JobPool {
	Mutex mrequest;
	std::list<Ptr<AJob>> requests;
	std::list<Ptr<AJob>>::iterator iterator;
	AsyncBuffer<AJob *, 16> queue;

	JobPool() : iterator(requests.begin()) {}
};

};
#endif // __task