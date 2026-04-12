#pragma once
#ifndef __singleton
#define __singleton


#include <unordered_map>
using namespace std;

template<typename T>
class Singleton {
protected:
	static inline T *inst = nullptr;

	Singleton() { 
		if (inst)
			throw;
		inst = static_cast<T*>(this);
	}
	~Singleton() = default;
	Singleton(Singleton &other) = delete;
	void operator=(Singleton &other) = delete;

public:
	static T *instance() { return inst; }
};

#endif // !__singleton