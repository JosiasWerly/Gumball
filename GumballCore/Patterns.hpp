#pragma once
#ifndef __patterns
#define __patterns

#include <iostream>
using namespace std;

struct PtrData{
	void *ptr = nullptr;
	unsigned int refCounter = 1;
};
template<class T>
class Var{
	template<class t> friend class Var;
	template<class t> friend Var<t> &&convertTo();

	PtrData *data = new PtrData;
	T **ptrCasted = (T **)&data->ptr;
protected:
	inline void ref() {
		data->refCounter++;
	}
	inline void unRef() {
		if (--data->refCounter == 0) {
			delete *ptrCasted;
			delete data;
		}
	}
	inline void setRef(PtrData *newData) {
		unRef();
		data = newData;
		ptrCasted = (T **)&data->ptr;
		ref();
	}
	
public:
	~Var() {
		unRef();
	}
	Var(T *init = nullptr){
		data->ptr = init;
		ptrCasted = (T**)&data->ptr;
	}
	Var(const Var &other) {
		setRef(other.data);
	}
	template<class t>Var(const Var<t>&other){
		setRef(other.data);
	}
	Var& operator=(const Var &other) {
		setRef(other.data);
		return *this;
	}
	

	T *operator*() { return data->ptr; }
	T *operator->() { return data->ptr; }
	//T &operator()() { return *static_cast<T*>(*data->ptr); }
	operator bool() const { return data->ptr;	}
	bool operator==(const Var &other) const { return data == other.data; }
	
	unsigned int referenceCount() const { return data->refCounter; }
};


template<typename T>
class Singleton {
protected:
	Singleton() {
	}
	~Singleton() {
	}
	Singleton(Singleton &other) = delete;
	void operator=(Singleton &other) = delete;
public:
	static T &instance() {
		static T instance;
		return instance;
	}
};

#endif // !__patterns