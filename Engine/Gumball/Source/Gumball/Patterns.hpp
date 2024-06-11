#pragma once
#ifndef _patterns
#define _patterns


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

#endif // !_patterns