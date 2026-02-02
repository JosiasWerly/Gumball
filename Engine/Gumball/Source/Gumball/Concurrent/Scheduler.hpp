#pragma once
#ifndef __scheduler
#define __scheduler

#include <thread>
#include <mutex>

#include "../Flow/Dispatcher.hpp"
#include "Common.hpp"

namespace Concurrent {
	using namespace std;

	struct GENGINE Runnable {
		const string name;

		Runnable(const Runnable &) = default;
		Runnable(const string &name) : name(name) {}
		bool operator==(const Runnable &o) const { return name == o.name; }
	};

	struct GENGINE Task : public Runnable {
		using Delegate = Signal<bool()>;
		Delegate fn;

		using Runnable::Runnable;
	};

	struct GENGINE Job : public Runnable {
		using Delegate = Signal<void()>;

		Delegate begin, end;
		std::list<Task> tasks;

		using Runnable::Runnable;
		Task &add(const string name);
		void pop(const string name);
		void clear();
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
		void markStart();
		void markTake();
		void markRelease();
		void markCompleted();

		bool isValid() const { return job && !job->tasks.empty(); }
		bool canStart() const { return state == eState::idle; }
		bool canTake() const { return !taken; }
		bool operator==(const Work &o) const { return this == &o; }
	};

	class GENGINE Scheduler {
		std::atomic<bool> active;
		vector<jthread> threads;

		std::condition_variable cvjobs;
		SharedList<Work> workPool;

		void run();

	public:
		Scheduler() = default;
		void add(Job &job);
		void pop(Job &job);
		void start(unsigned threadCount);
		void stop();
	};
};

#endif // __scheduler
