#pragma once
#ifndef __concurrentcommon
#define __concurrentcommon

#include <mutex>
#include <atomic>
#include <array>
namespace Concurrent {
	using Mutex = std::mutex;
	using Lock = std::scoped_lock<Mutex>;
	using ULock = std::unique_lock<Mutex>;
	using CVar = std::condition_variable;
	template<class T> using Atomic = std::atomic<T>;

	template <typename T, unsigned Size>
	class AsyncBuffer {
		Atomic<unsigned> head{ 0 };
		Atomic<unsigned> tail{ 0 };
		std::array<Atomic<short>, Size> seq;
		std::array<T, Size> arr;

	public:
		AsyncBuffer() {
			for (int j = 0; j < Size; ++j)
				seq[j] = j;
		}
		bool Push(const T &value) {
			const unsigned h = head.load(std::memory_order_relaxed);
			const unsigned lh = h % Size;

			const unsigned t = tail.load(std::memory_order_acquire);
			if (seq[lh].load(std::memory_order_acquire) != h)
				return false;
			arr[lh] = std::move(value);
			seq[lh].store(h + 1, std::memory_order_release);
			head.fetch_add(1, std::memory_order_release);
			return true;
		}
		bool Pop(T &value) {
			const unsigned t = tail.load(std::memory_order_relaxed);
			while (true) {
				unsigned lt = t % Size;
				const short s = seq[lt].load(std::memory_order_acquire);
				if (t + 1 != s)
					return false;

				unsigned nt = t;
				if (tail.compare_exchange_weak(nt, nt + 1, std::memory_order_relaxed, std::memory_order_relaxed)) {
					value = arr[lt];
					seq[lt].store(t + Size, std::memory_order_release);
					return true;
				}
			}
			return false;
		}
		bool Full() const {
			const unsigned h = head.load(std::memory_order_acquire);
			const unsigned t = tail.load(std::memory_order_acquire);
			const unsigned cap = Size - 1;
			if (h == t)
				return false;
			return t < h ? h - t == cap : (Size - t) + h == cap;
		}
		bool Empty() const { return head.load(std::memory_order_acquire) == tail.load(std::memory_order_acquire); }
	};
};
#endif // __concurrentcommon
