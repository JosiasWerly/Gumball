#pragma once
#ifndef __codex
#define __codex

#include <unordered_map>
#include <typeinfo>

class GENGINE Codex {
	std::unordered_map<size_t, void *> data;

public:
	void add(size_t hash, void *ptr) {
		data.insert({ hash, ptr });
	}
	template<class T> void add(T *ptr) {
		data.insert({ typeid(T).hash_code(), ptr });
	}
	template<class T> T *get() {
		return reinterpret_cast<T *>(data[typeid(T).hash_code()]);
	}
};

#endif // !__codex