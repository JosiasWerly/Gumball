#pragma once
#ifndef _patterns
#define _patterns


template<typename T>
class Singleton {
protected:
	Singleton() {}
	~Singleton() {}
	Singleton(Singleton& other) = delete;
	void operator=(Singleton& other) = delete;
public:
	static T& instance() {
		static T instance;
		return instance;
	}
};

#endif // !_patterns