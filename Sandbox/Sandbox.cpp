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




//namespace Concurrent {
//	class Thread;
//	
//	using Callback = Signal<void()>;
//	using GuardLock = std::lock_guard<std::mutex>;
//	using GuardUnique = std::unique_lock<std::mutex>;
//	using GuardScoped = std::scoped_lock<std::mutex>;
//
//
//
//	class ThreadInfo : public Singleton<ThreadInfo> {
//		friend class Thread;
//		static thread_local Thread *localThread;
//	
//	public:
//		static Thread *local() { return localThread; }
//	};
//	thread_local Thread *ThreadInfo::localThread = nullptr;
//	
//	struct IRunnable {
//		virtual void execute() = 0;
//	};
//	struct Schedule : public IRunnable {
//		void execute() override {}
//	};
//	struct Job : public IRunnable {
//		Callback cb;
//		void execute() override { cb(); }
//	};
//
//	class Thread {
//		friend class MainThread;
//
//		jthread th;
//		TPtr<IRunnable> rn;
//		atomic<bool> running;
//		Thread *parent;
//		list<Thread*>children;
//
//		void propagateStop(Thread *root, list<Thread *> &all) {
//			root->th.request_stop();
//			for (auto &n : root->children) {
//				all.push_back(n);
//				propagateStop(n, all);
//			}
//		}
//		void run() {
//			ThreadInfo::localThread = this;
//			do { rn->execute(); } while (!th.get_stop_token().stop_requested());
//			ThreadInfo::localThread = nullptr;
//			running = false;
//		}
//	
//	public:
//		Thread() = default;
//		void start(bool singleRun = false) {
//			running = true;
//			th = jthread(&Thread::run, this);
//			if (singleRun)
//				stop();
//		}
//		void stop() {
//			list<Thread *> related;
//			propagateStop(this, related);
//			{
//				list<Thread *> stopping = related;
//				for (list<Thread *>::iterator it = related.begin(); related.size(); ) {
//					Thread *leaf = (*it);
//					if (leaf->children.empty() && leaf->th.joinable()) {
//						leaf->th.join();						
//						it = leaf->parent->children.erase(it);
//					}
//					else
//						++it;
//				}
//			}
//		}
//		bool isRunning() const { return running; }
//		bool operator==(const Thread &other) const { return th.get_id() == other.th.get_id(); }
//		
//		
//		TPtr<IRunnable> &work () { return rn; }
//	};
//
//};

//using namespace Concurrent;
//
//static int arr[1024];
//void generate() {
//	cout << __FUNCTION__ << endl;
//	for (int i = 0; i < 1024; ++i) {
//		arr[i] = rand() % 100 + 1;
//		//this_thread::sleep_for(1ms);
//	}
//}
//void sort() {
//	cout << __FUNCTION__ << endl;
//	for (int i = 0; i < 1024; ++i) {
//		arr[i] = rand() % 100 + 1;
//		//this_thread::sleep_for(1ms);
//	}
//}
//void sum() {
//	this_thread::sleep_for(std::chrono::duration<long double, std::milli>(5000));
//	//Thread *w = MainThread::instance()->local();
//	//w->stop();
//	cout << __FUNCTION__ << endl;
//}
//void sub() {
//	this_thread::sleep_for(std::chrono::duration<long double, std::milli>(2000));
//	//MainThread::instance()->local()->stop();
//	cout << __FUNCTION__ << endl;
//}
//void async() {
//	cout << __FUNCTION__ << endl;
//	//Thread *cur = MainThread::local();
//	//
//	//Thread &wsum = MainThread::instance()->newThread();
//	//wsum.target().bind(sum);
//	//wsum.start();
//	//
//	//Thread &wsub = MainThread::instance()->newThread();
//	//wsub.target().bind(sub);
//	//wsub.start();
//	
//	//while (wsum.isRunning() || wsub.isRunning());
//}
//void conclude() {
//	cout << __FUNCTION__ << endl;
//	//MainThread::instance()->local()->stop();
//}
//
//void A() {
//	static int i = 0;
//	cout << __FUNCTION__ << endl;
//	this_thread::sleep_for(1ms);
//	//if(++i == 10)
//	ThreadInfo::local()->stop();
//}
//void B() {
//	static int i = 0;
//	cout << __FUNCTION__ << endl;
//	this_thread::sleep_for(1ms);
//	if (++i == 100)
//		ThreadInfo::local()->stop();
//}


void render() {
	//Thread &thRender = MainThread::instance()->newThread();
	//thRender.sche().bind(generate);
	//thRender.sche().bind(sort);
	//thRender.sche().bind(async);
	//thRender.sche().bind(conclude);
	//thRender.start();
}

Extern GGAME void *EntryPoint() {
	//Thread a, b;
	//a.work() = new Job{
	//	
	//};
	//b.work() = new Schedule{

	//};

	//a.start(); 
	//b.start();
	//while (a.isRunning() || b.isRunning());

	cout << "work complete" << endl;
	return new MyProject;
}