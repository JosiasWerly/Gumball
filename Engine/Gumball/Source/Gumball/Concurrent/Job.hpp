#pragma once
#ifndef __job
#define __job

#include <Gumball/Containers/Delegate.hpp>
#include <Gumball/Containers/Pointer.hpp>
#include "Common.hpp"

namespace Concurrent {
using namespace std;

class AJob {
	friend class Job;
	friend class Scheduler;
	using FRun = Delegate<void(AJob &)>;
	enum class eState : char { Idle, Waiting, Scheduled, Done };

	Atomic<eState> state{ eState::Idle };

	FRun run;
	void *data = nullptr;
	inline void Run() { run(*this); }
};

class GENGINE Job {
	Ptr<AJob> hjob;

public:
	Job();
	void Start();
	void Stop();
	bool Began() const;
	inline bool operator==(const AJob &h) const { return &(*hjob) == &h; }
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