#pragma once
#ifndef __scheduler
#define __scheduler

#include <thread>
#include <mutex>

#include "../Flow/Dispatcher.hpp"
#include "Common.hpp"

int main(int argc, char *argv[]);

namespace Concurrent {
	using namespace std;

	struct GENGINE Task {
		using Delegate = Signal<bool()>;
		Delegate fn;

		bool operator==(const Task &other) const { return this == &other; }
	};

	struct GENGINE Job {
		using Delegate = Signal<void(Job *)>;
		
		void *data = nullptr;
		Delegate begin, end;
		std::list<Task> tasks;

		Task *Add();
		void Pop(Task *task);
		void Clear();
		bool operator==(const Job &other) const { return this == &other; }
	};

	struct GENGINE Work {
		enum class eState { idle, schedule };

		eState state = eState::idle;
		Job *job = nullptr;
		unsigned tasksMask = 0;
		unsigned char tasksCompleted = 0;
		bool taken = false;

		Work() = default;
		Work(Work &o);
		void MarkStart();
		void MarkTake();
		void MarkRelease();
		void MarkCompleted();

		bool IsValid() const { return job && !job->tasks.empty(); }
		bool CanStart() const { return state == eState::idle; }
		bool CanTake() const { return !taken; }
		bool operator==(const Work &o) const { return this == &o; }
	};

	class GENGINE Scheduler {
		friend int ::main(int argc, char *argv[]);

		std::atomic<bool> active;
		vector<jthread> threads;

		std::condition_variable cvjobs;
		SharedList<Work> workPool;
		SharedList<Job> jobPool;

		void Run();
		void RunMainThread() { while (true); }
	public:
		Scheduler() = default;
		Job* Add();
		void Pop(Job *job);
		void Start(unsigned threadCount);
		void Stop();
	};
};

#endif // __scheduler
