#include "Sandbox.hpp"

#include <Asset/Asset.module.hpp>
#include <Render/Render.module.hpp>
#include <Gumball/Domain.hpp>
#include <Gumball/Engine.hpp>

#include <Render/Scene/SceneOverlay.hpp>
#include <Render/Widget/WidgetOverlay.hpp>
#include <Input/Input.module.hpp>
#include <World/Actor.hpp>

#include <Gumball/Flow/Common.hpp>
#include <Gumball/Containers/Pointer.hpp>

#include <iostream>
#include <list>
#include <memory>
#include <functional>

#include <chrono>
#include <thread>
#include <mutex>
#include <future>



class GGAME MyComp : public MeshComponent {
public:
	Vector3 vel;
	MyComp() {
		setTickEnable(true);
	}
	void beginPlay() {
		MeshComponent::beginPlay();
		owner->transform.scale = Vector3(1, 1, 1);
		vel = Vector3(rand() % 2 + 1, rand() % 2 + 1, rand() % 2 + 1);
		vel = vel.normalize() * 0.01f;
		if (rand() % 2) {
			vel *= -1;
		}
	}
	void tick(const double &deltaTime) {
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		owner->transform.rotation.rotate(.05f, .05f, .05f);
		owner->transform.position += vel;
		auto &pos = owner->transform.position;
		if (pos.x < -10) {
			pos.x = -10;
			vel.x *= -1;
		}
		else if (pos.x > 10) {
			pos.x = 10;
			vel.x *= -1;
		}

		if (pos.y < -10) {
			pos.y = -10;
			vel.y *= -1;
		}
		else if (pos.y > 10) {
			pos.y = 10;
			vel.y *= -1;
		}
		if (pos.z < -10) {
			pos.z = -10;
			vel.z *= -1;
		}
		else if (pos.z > 10) {
			pos.z = 10;
			vel.z *= -1;
		}
	}
};

static int i = 0;
static Actor *camera = nullptr;
class GGAME MyActor :
	public Actor {
public:
	//DrawCallInstance *di;
	MyActor() {
		//di = new DrawCallInstance("torus");
		//di->transform = &transform;
		transform.scale = Vector3(int(0.001), int(0.001), int(0.001));
		transform.position = Vector3(-5 + i, 0, 0);
		i += 3;
		addComponent(new MyComp);
		setTickEnable(true);
	}
	virtual ~MyActor() {
		//delete di;
		//di = nullptr;
	}
	void beginPlay() override {
		Actor::beginPlay();
	}
	void endPlay() override {
		Actor::endPlay();
	}
	void tick(const double &deltaTime) {
		Actor::tick(deltaTime);
		
		if (Input::isKeyDown(Input::EKeyCode::A)) {
			camera->transform.rotation.rotate(0, 0, 1.);
		}
		else if (Input::isKeyDown(Input::EKeyCode::D)) {
			camera->transform.rotation.rotate(0, 0, -1.);
		}
		if (Input::isKeyDown(Input::EKeyCode::W)) {
			camera->transform.rotation.rotate(0, 1, 0);
		}
		else if (Input::isKeyDown(Input::EKeyCode::S)) {
			camera->transform.rotation.rotate(0, -1, 0);
		}
	}
};

class GGAME MyProject : public Project {
public:	
	void attached() {
		cout << "loaded" << endl;
		srand(0);

		AssetModule *m = AssetModule::instance();
		WorldModule *w = WorldModule::instance();
		RenderModule *r = RenderModule::instance();
		InputModule *i = InputModule::instance();
		Domain *d = Domain::instance();

		auto cameraActor = new Actor;
		camera = cameraActor;
		{
			auto cameraComp = new CameraComponent;
			cameraActor->addComponent(cameraComp);
			cameraActor->transform.position.z = -20;
		}

		for (size_t i = 0; i < 5; i++) {
			new MyActor;
		}
	}
	void detached() {
		cout << "unloaded" << endl;
	}
};


namespace Concurrent {
	Scheduler::Scheduler() : 
		queue(&q1), nqueue(&q2) {
	}
	void Scheduler::run() {
		while (true) {
			Task *task = nullptr;
			{
				GuardUnique lock(mqueue);
				cvqueue.wait(lock, [this]() { return !active || !queue->empty(); });
				if (!active)
					return;
				task = queue->front();
				queue->pop();
			}
			if (!task) continue;
			task->fn();
			
			if (task->hasNotify()) {
				GuardUnique lock(mqueue);
				task->notify(*nqueue);
				task->clear();
				if (queue->empty() && !nqueue->empty()) {
					std::queue<Task *> *temp = queue;
					queue = nqueue;
					nqueue = temp;
				}
			}
		}
	}
	void Scheduler::add(TaskHandler &task) {
		std::unique_lock lock(mqueue);
		task.tsk = &tasks.emplace_back();
		queue->push(task.tsk);
	}
	void Scheduler::pop(TaskHandler &task) {
		std::unique_lock lock(mqueue);
	}
	void Scheduler::start(unsigned threadCount) {
		active = true;
		for (size_t i = 0; i < threadCount; i++)
			threads.emplace_back(jthread(&Scheduler::run, this));
	}
	void Scheduler::stop() {
		active = false;
	}


	void Task::pushRight(Task &trg) {
		trg.right.push_back(this);
		notified[&trg] = false;
	}
	void Task::popRight(Task &trg) {
		trg.right.remove(this);
		notified.erase(&trg);
	}
	void Task::notify(std::queue<Task *> &ready) {
		for (auto &r : right) {
			r->notified[this].store(true);
			if (r->canRun())
				ready.push(r);
		}
	}
	void Task::clear() {
		for (auto &[p, flag] : notified)
			flag.store(false);
	}
	bool Task::canRun() const {
		for (auto &[p, flag] : notified)
			if (!flag.load())
				return false;
		return true;
	}
	bool Task::hasNotify() const {
		return !right.empty();
	}

	TaskHandler &TaskHandler::operator<<(TaskHandler &other) {
		tsk->pushRight(*other.tsk);
		return *this;
	}
	TaskHandler &TaskHandler::operator>>(TaskHandler &other) {
		tsk->popRight(*other.tsk);
		return *this;
	}
};


using namespace Concurrent;

static int arr[1024];
void A() {
	Trace;
}
void B() {
	Trace;
}
void C() {
	Trace;
}
void D() {
	Trace;
}
void E() {
	Trace;
}
void F() {
	Trace;
}

Extern GGAME void *EntryPoint() {
	
	Scheduler sc;
	{
		std::vector<TaskHandler> j(6);
		for (int i = 0; i < 6; ++i)
			sc.add(j[i]);
		j[0].callback().bind(&A);
		j[1].callback().bind(&B);
		j[2].callback().bind(&C);
		j[3].callback().bind(&D);
		j[4].callback().bind(&E);
		j[5].callback().bind(&F);

		j[0] << j[2] << j[3] << j[4];
		j[1] << j[0];
		
		sc.start();
		while (true);
	}
	cout << "work complete" << endl;
	return new MyProject;
}


//namespace Concurrent {
//
//	using Delegate = Signal<void()>;
//
//	using Mutex = std::mutex;
//	using GuardLock = std::lock_guard<std::mutex>;
//	using GuardUnique = std::unique_lock<std::mutex>;
//	using GuardScoped = std::scoped_lock<std::mutex>;
//	template<class T> using Writer = std::promise<T>;
//	template<class T> using Reader = std::future<T>;
//
//
//	thread_local Thread *Thread::localThread = nullptr;
//
//	struct Work;
//	using WorkSignal = unordered_map<Work *, bool>;
//
//
//	struct Work {
//		struct Sync {
//			WorkSignal *emit;
//			WorkSignal receive;
//
//			void complete(Work *w) {
//				(*emit)[w] = true;
//
//			}
//		};
//
//		void *data;
//		Delegate work;
//		Sync *sync;
//
//		void complete() {
//			if (sync) {
//				sync->complete();
//			}
//		}
//		bool canRun() const {
//			bool out = true;
//			for (auto kv : receive) {
//				if (!kv.second)
//					return false;
//			}
//			return true;
//		}
//	};
//
//	struct Job {
//		Mutex mjobs;
//		int curJob = 0;
//		vector<Work> works;
//
//		Job() = default;
//		void lock() { mjobs.lock(); }
//		void unlock() { mjobs.unlock(); }
//		Work *operator*() { return curJob > -1 && curJob < works.size() ? &works[curJob] : nullptr; }
//		bool operator++() { return ++curJob < works.size(); }
//		bool operator--() { return --curJob > -1; }
//	};
//
//	struct Service {
//
//	};
//
//	class Schedule {
//		vector<Job> jobs;
//		vector<Sync> syncronization;
//
//		void tick() {
//
//		}
//	};
//};