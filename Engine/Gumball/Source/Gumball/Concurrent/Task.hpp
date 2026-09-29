#pragma once
#ifndef __task
#define __task

#include <Gumball/Containers/Dispatcher.hpp>
#include <Gumball/Containers/Pointer.hpp>
#include "Common.hpp"

namespace Concurrent {
using namespace std;

class ATask {
	friend class Task;
	friend class Scheduler;
	using FRun = Signal<void(ATask &)>;
	using FEnd = Signal<void(const ATask &)>;
	enum class eState : char { Idle, Waiting, Scheduled, Done };
	enum class eResult : char { Continue, Success, Failed };

	Atomic<eState> state{ eState::Idle };
	eResult result;
	FRun run;
	FEnd end;
	PtrVoid data;

public:
	inline PtrVoid &Data() { return data; }
	inline const PtrVoid &Data() const { return data; }
	inline void Complete(bool success) { result = success ? eResult::Success : eResult::Failed; }
	inline bool IsSuccess() const { return result == eResult::Success; }
	inline bool IsCompleted() const { return result != eResult::Continue; }
};

class Task {
protected:
	Ptr<ATask> htask;

public:
	Task();
	Task(ATask::FRun frun, ATask::FEnd fend, void *data);
	void Bind(ATask::FRun frun, ATask::FEnd fend, void *data = nullptr);
	void Start();
	void Stop();

	ATask::FRun &Run() { return htask->run; }
	ATask::FEnd &End() { return htask->end; }
	inline PtrVoid &Data() { return htask->Data(); }
	inline const PtrVoid &Data() const { return htask->Data(); }
	inline void Complete(bool success) { htask->Complete(success); }
	inline bool IsSuccess() const { return htask->IsSuccess(); }
	inline bool IsCompleted() const { return htask->IsCompleted(); }
	bool Began() const;

	inline bool operator==(const ATask &h) const { return (void*)htask == &h; }
	operator bool() const { return htask->run || htask->end; }
};

class TaskSequence : public Task {
	using Task::Stop;
	using Task::Start;
	using Task::Run;
	using Task::Bind;
	
	std::list<Task> tasks;	
	void OnTaskCompleted(const ATask &);
public:
	TaskSequence() = default;
	void Start();
	void Push(const Task &entry) { tasks.push_back(entry); }
	operator bool() const { return !tasks.empty(); }
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