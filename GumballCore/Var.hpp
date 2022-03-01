#pragma once
#ifndef _var
#define _var

#include <iostream> 
using namespace std;

struct PContainer {
	unsigned int refCounter = 1;
	void *ptr;
	PContainer(void *ptr = nullptr) : 
		ptr(ptr) {
		cout << "()" << this << endl;
	}
	~PContainer() {
		cout << "~" << this << endl;
	}
};
template<class T>
class Var {
	template<class t> friend class Var;

	PContainer *container;
	T *tptr;
protected:
	inline void release() {
		if (--container->refCounter == 0) {
			delete tptr;
			delete container;
			tptr = nullptr;
			container = nullptr;
		}
	}
	inline void changeRef(PContainer *newContainer) {
		release();
		container = newContainer;
		tptr = (T *)container->ptr;
		container->refCounter++;
	}

public:
	~Var() {
		release();
	}
	Var(T *init = nullptr) :
		container(new PContainer(init)),
		tptr((T *)container->ptr) {
	}
	Var(const Var &other) :
		container(other.container),
		tptr((T *)container->ptr) {
		++container->refCounter;
	}
	Var(Var &&other) : 
		container(other.container),
		tptr((T *)container->ptr) {
		++container->refCounter;
	}

	template<class t>Var(const Var<t> &other) {
		changeRef(other.container);
	}
	Var &operator=(const Var &other) {
		changeRef(other.container);
		return *this;
	}


	T *&operator*() { return tptr; }
	T *&operator->() { return tptr; }
	operator bool() const { return tptr; }
	bool operator==(const Var &other) const { return container == other.container; }

	unsigned int referenceCount() const { return container->refCounter; }
	template<class t> operator Var<t>() {
		Var<t> out;
		out.changeRef(container);
		return out;
	}
	template<class t> t* As() {
		return dynamic_cast<t*>(tptr);
	}
};
#endif // !_var