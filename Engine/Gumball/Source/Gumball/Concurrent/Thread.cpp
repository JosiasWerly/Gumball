#include "Thread.hpp"

using namespace Concurrent;

Thread::~Thread() {
	stop(true);
}
void Thread::run() {
	Thread::localThread = this;
	do { dl(); } while (!isStopping());
	Thread::localThread = nullptr;
	running = false;
}
void Thread::start() {
	running = true;
	th = std::jthread(&Thread::run, this);
}
void Thread::stop(bool block) {
	th.request_stop();
	if (block) {
		while (isRunning());
		th.join();
	}
}