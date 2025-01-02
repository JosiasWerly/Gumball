#pragma once
#ifndef _var
#define _var
#include <iostream>

class VarTarget {
public:
	virtual ~VarTarget() {}
};
template<class T> class TVarTarget : public VarTarget {
public:
	T *ptr = nullptr;
	TVarTarget(T *ptr) : 
		ptr(ptr) {
	}
	~TVarTarget() {
		delete ptr;
		ptr = nullptr;
	}
};
class VarContainer {
public:
	unsigned references = 1;
	VarTarget *var = nullptr;

	VarContainer(VarTarget *var) :
		var(var) {
	}
	~VarContainer() {
		delete var;
	}
};


class Var {
protected:
	Var() {}

public:
	virtual ~Var() {}
	virtual bool isValid() { return false; }
};

template<class T> class TVar : public Var {
	typedef VarContainer Container;
	template<class t> friend class TVar;

protected:
	Container *container = nullptr;
	T *&ptr;

	Inline void unRef() {
		if (--container->references == 0) {
			delete container;
			container = nullptr;
		}
	}
	Inline void addRef(const TVar &newTarget) {
		setRef(newTarget);
		++container->references;
	}
	Inline void setRef(const TVar &newTarget) {
		container = newTarget.container;
		ptr = newTarget.ptr;
	}
	Inline T *&getRefTracker() {
		if (auto trg = dynamic_cast<TVarTarget<T>*>(container->var))
			return trg->ptr;
		else
			throw;
	}
	

	TVar(Container *container, long ptrAddress) :
		container(container),
		ptr((T*&)ptrAddress) {
	}
public:
	virtual ~TVar() {
		unRef();
	}
	TVar(const TVar &other) :
		ptr(other.ptr) {
		addRef(other);
	}
	TVar(TVar &&other) noexcept :
		ptr(other.ptr) {
		addRef(other);
	}
	TVar(T *newValue = nullptr) :
		container(new Container(new TVarTarget<T>(newValue))),
		ptr(dynamic_cast<TVarTarget<T>*>(container->var)->ptr) {
	}
	TVar &operator=(const TVar &other) {
		unRef();
		addRef(other);
		return *this;
	}

	T &operator*() { return *ptr; }
	T *&operator->() { return ptr; }
	bool isValid() override { return *this; }
	operator bool() { return container && container->var && ptr; }
	bool operator==(const TVar &other) const { return container == other.container; }
	Inline unsigned references() { container->references; }
	Inline T *pin() { return ptr; }
	template<class t> t *pin() { return static_cast<t *>(ptr); }

	void free() {
		T *&ptr = getRefTracker();
		delete ptr;
		ptr = nullptr;
	}
	void assign(T *init) {
		free();
		getRefTracker() = init;
	}
};
#endif
//Var<Object> b;
//{
//	Var<Object> a, c = b;
//	a = new Foo;
//	a.As<Foo>()->a = 1;
//	a.As<Foo>()->b = 2.2;
//
//	b = a;
//	{
//		Var<Foo> af = a;
//		a.As<Foo>()->test();
//	}
//	delete *b;
//	*b = nullptr;
//	*b = new Foo;
//}