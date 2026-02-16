#pragma once
#ifndef __concurrentcommon
#define __concurrentcommon

#include <mutex>
#include <list>
namespace Concurrent {
	using Mutex = std::mutex;
	using GuardLock = std::lock_guard<std::mutex>;
	using GuardUnique = std::unique_lock<std::mutex>;
	using GuardScoped = std::scoped_lock<std::mutex>;

	template<class T>
	struct SharedList {
		std::list<T>::iterator it;
		std::list<T> pool;
		Mutex m;

		SharedList() { it = pool.end(); }
		SharedList(SharedList &o) {
			GuardLock l(m);
			GuardLock k(o.m);
			it = o.it;
			pool = o.pool;
		}
		T &add(T &&t = T()) {
			pool.emplace_back(t);
			if (pool.size() == 1) {
				it = pool.begin();
			}
			return pool.back();
		}
		void pop(T &t) {
			if ((*it) == t)
				it = pool.erase(it);
			else
				pool.remove(t);
		}

		T *next() {
			if (isEmpty())
				return nullptr;

			T *out = &(*it);
			if (++it == pool.end())
				it = pool.begin();

			return out;
		}

		bool isLast() const { return it == pool.end(); }
		bool isEmpty() const { return pool.empty(); }
	};

	class SpinLock {
		std::atomic_flag flag = ATOMIC_FLAG_INIT;

	public:
		void lock() noexcept {
			while (flag.test_and_set(std::memory_order_acquire))
				;
		}
		bool try_lock() noexcept {
			return !flag.test_and_set(std::memory_order_acquire);
		}
		void unlock() noexcept {
			flag.clear(std::memory_order_release);
		}
	};
};
#endif // __concurrentcommon
