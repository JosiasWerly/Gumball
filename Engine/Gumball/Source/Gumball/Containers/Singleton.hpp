#pragma once
#ifndef __SINGLETON
#define __SINGLETON

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
	static T &Instance() { return *inst; }
};

template<class T, T& (*fn)(void)>
class Global {
protected:
	Global() = default;
	virtual ~Global() = default;

public:
	static T &Instance() {
		static T *inst = &fn();
		return *inst;
	}
};

#endif // !__SINGLETON