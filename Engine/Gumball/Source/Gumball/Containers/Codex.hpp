#pragma once
#ifndef __codex
#define __codex
#include <typeinfo>
#include <atomic>
#include <unordered_map>

namespace Containers {

class GENGINE TypeCodex {
	std::unordered_map<size_t, void *> data;

public:
	void Add(size_t hash, void *ptr) {
		data.insert({ hash, ptr });
	}
	template<class T> T* Add(T *ptr = new T) {
		data.insert({ typeid(T).hash_code(), ptr });
		return ptr;
	}
	template<class T> T *Get() {
		return reinterpret_cast<T *>(data[typeid(T).hash_code()]);
	}
};

class GENGINE Codex {
    template<class T>
    static std::atomic<T *> &Instance() {
        static std::atomic<T *> instance{ nullptr };
        return instance;
    }

public:
    constexpr Codex() = default;
    template<class T>
    static T *Get() {
        return Instance<T>().load(std::memory_order_acquire);
    }
    
    template<class T>
    static T *Add(T *ptr = new T) {
        T *expected = nullptr;
        const bool exchange = Instance<T>().compare_exchange_strong(expected, ptr, std::memory_order_release, std::memory_order_relaxed);
        if (exchange)
            return ptr;
        else
            delete ptr;
        return expected;
    }
};

};
#endif // !__codex