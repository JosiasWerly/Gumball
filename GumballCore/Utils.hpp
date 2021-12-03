#pragma once
#ifndef __utils
#define __utils

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
	
	unsigned int referenceCount() { return data->refCounter; }

	T *operator*() { return data->ptr; }
	T *operator->() { return data->ptr; }
	operator bool() { return data->ptr;	}
	bool operator ==(Var &other) { return data == other.data; }
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



#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shaders.hpp"

namespace Conversors {

template<class T> constexpr unsigned typeCToGL() = delete;
#define pushType(CType, GLType)template<> constexpr unsigned typeCToGL<CType>() { return GLType; }
pushType(char *, GL_BYTE)
pushType(bool, GL_BOOL)
pushType(int, GL_INT)
pushType(float, GL_FLOAT)


#undef pushType
template<int> constexpr unsigned sizeGLToC() = delete;
#define pushType(GLType, CSize)template<> constexpr unsigned sizeGLToC<GLType>() { return CSize; }
pushType(GL_FLOAT, 4)
pushType(GL_INT, 4)
pushType(GL_BYTE, 1)
#undef pushType
};

#endif // !__utils