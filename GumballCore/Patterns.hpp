#pragma once
#ifndef _patterns
#define _patterns

template<typename T>
class Singleton {
protected:
	static inline T *inst = nullptr;


	Singleton() {
	}
	~Singleton() {
	}
	Singleton(Singleton &other) = delete;
	void operator=(Singleton &other) = delete;
public:
	static T *setInstance(T *newInstance) {
		return inst = newInstance;
	}
	static T *instance() {
		return Singleton::inst;
	}
};

#endif // !_patterns