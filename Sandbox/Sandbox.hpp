#include <Gumball/ProjectTarget.hpp>

//#include <GumballCore/Activator.hpp>

//struct ActivatorPackage_Project : public ActivatorPackage {
//	ActivatorPackage_Project();
//};

Extern GGAME void *EntryPoint();


#include <queue>
#include <condition_variable>
namespace Concurrent {
	using namespace Flow;
	using Delegate = Signal<void()>;

	using Mutex = std::mutex;
	using GuardLock = std::lock_guard<std::mutex>;
	using GuardUnique = std::unique_lock<std::mutex>;
	using GuardScoped = std::scoped_lock<std::mutex>;

	struct SharedData;
	struct Task;
	class TaskHandler;
	class Scheduler;

	struct SharedData {
		enum class eState : char { Write, Read };

		void *data = nullptr;
		//unordered_map<Job *, bool> writers;
		//list<Job *, bool> readers;

		bool isWritable() const { return false; }
	};
	
	class Scheduler {
		vector<jthread> threads;

		Mutex mqueue;
		std::condition_variable cvqueue;
		std::list<Task> tasks;
		std::queue<Task *> q1, q2;
		std::queue<Task *> *queue, *nqueue;
		std::atomic<bool> active;
		
		void run();

	public:
		Scheduler();
		void add(TaskHandler &task);
		void pop(TaskHandler &task);
		void start(unsigned threadCount = 4);
		void stop();
	};

	struct Task {		
		Delegate fn;
		list<Task *> right;
		unordered_map<Task *, std::atomic<bool>> notified;

		Task() = default;
		void pushRight(Task &trg);
		void popRight(Task &trg);
		void notify(std::queue<Task *> &ready);
		void clear();
		bool canRun() const;
		bool hasNotify() const;
	};

	class TaskHandler {
		friend class Scheduler;
		Task *tsk = nullptr;

	public:		
		TaskHandler() = default;
		~TaskHandler() = default;

		Delegate &callback() { return tsk->fn; }
		TaskHandler &operator<<(TaskHandler &other);
		TaskHandler &operator>>(TaskHandler &other);
	};

	#define Trace \
	cout << __FUNCTION__ << endl;\
	this_thread::sleep_for(std::chrono::duration<long double, std::milli>(500))
};