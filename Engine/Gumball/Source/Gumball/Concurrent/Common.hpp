#pragma once
#ifndef __concurrentcommon
#define __concurrentcommon

#include <mutex>
#include <list>
#include <array>
namespace Concurrent {
	using Mutex = std::mutex;
	using Lock = std::scoped_lock<Mutex>;
	using ULock = std::unique_lock<Mutex>;
	using CVar = std::condition_variable;
	template<class T> using Atomic = std::atomic<T>;

	template <typename T, unsigned TCapacity>
	class TRingBuffer {
		Atomic<unsigned> head{ 0 };
		Atomic<unsigned> tail{ 0 };
		std::array<T, TCapacity> buffer;

	public:
		bool Push(T value) {
			const unsigned h = head.load(std::memory_order_acquire);
			const unsigned t = tail.load(std::memory_order_relaxed);
			const unsigned nt = (t + 1) % TCapacity;
			if (nt == h)
				return false;

			buffer[t] = std::move(value);
			tail.store(nt, std::memory_order_release);
			return true;
		}
		T Pop() {
			unsigned h = head.load(std::memory_order_relaxed);
			while (true) {
				const unsigned t = tail.load(std::memory_order_acquire);
				if (h == t)
					return nullptr;

				const unsigned nh = (h + 1) % TCapacity;
				if (head.compare_exchange_weak(h, nh, std::memory_order_acquire, std::memory_order_relaxed))
					return buffer[h];
			}
		}
		bool Full() const {
			const unsigned h = head.load(std::memory_order_acquire);
			const unsigned t = tail.load(std::memory_order_acquire);
			const unsigned cap = TCapacity - 1;
			if (h == t)
				return false;
			return h < t ? (t - h) == cap : (TCapacity - h) + t == cap;
		}
		bool Empty() const { return head.load(std::memory_order_acquire) == tail.load(std::memory_order_acquire); }
	};

	template<class T>
	struct TPool {
		Mutex mrequest;
		std::list<Ptr<T>> requests;
		TRingBuffer<T *, 4> queue;
	};
};
#endif // __concurrentcommon
