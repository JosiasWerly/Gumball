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
};

#endif // __concurrentcommon
