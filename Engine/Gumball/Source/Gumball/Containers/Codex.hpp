#pragma once
#ifndef __codex
#define __codex
#include <typeinfo>
#include <atomic>
#include <unordered_map>

namespace Containers {

class GENGINE Codex {
	std::unordered_map<size_t, void *> data;

public:
	void Add(size_t hash, void *ptr) {
		data.insert({ hash, ptr });
	}
	template<class T> T& Add(T *ptr = new T) {
		data.insert({ typeid(T).hash_code(), ptr });
		return *ptr;
	}
	template<class T> T &Get() {
		return *reinterpret_cast<T *>(data[typeid(T).hash_code()]);
	}
};
};
#endif // !__codex