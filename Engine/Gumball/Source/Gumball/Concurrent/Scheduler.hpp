#pragma once
#ifndef __scheduler
#define __scheduler

#include <thread>
#include <Gumball/Containers/Pointer.hpp>
#include <Gumball/Concurrent/Task.hpp>
#include "Common.hpp"

int main(int argc, char *argv[]);

namespace Concurrent {
using namespace std;

class GENGINE Scheduler {
	friend int ::main(int argc, char *argv[]);

	const unsigned threadCount = 3;
	CVar cvthread;
	Mutex mthread;
	std::list<std::jthread> threads;

	TPool<ATask> tasks;

	void Tick();
	void ConsumerTick();

public:
	Scheduler();
	void Initialize(unsigned char thCount);
	void Shutdown();
	
	void Add(Ptr<ATask> &task);
	void Pop(Ptr<ATask> &task);
};

};
#endif // __scheduler