#pragma once
#ifndef __thread
#define __thread

#include <thread>
#include <mutex>
#include "Flow/Dispatcher.hpp"
namespace Concurrent {
	class Thread {
		using Delegate = Signal<void()>;
	
	private:
		static thread_local Thread *localThread;
		Delegate dl;
		std::jthread th;
		std::atomic<bool> running;

		void run();
	public:
		Thread() = default;
		Thread(Thread &&) = delete;
		Thread(const Thread &) = delete;
		~Thread();
		void start();
		void stop(bool block = false);
		Delegate &caller() { return dl; }

		bool isRunning() const { return running; }
		bool isStopping() const { return th.get_stop_token().stop_requested(); }
		bool operator==(const Thread &other) const { return th.get_id() == other.th.get_id(); }
		const Delegate &caller() const { return dl; }

		static Thread *local() { return localThread; }
	};
	thread_local Thread *Thread::localThread = nullptr;
};
#endif // __thread