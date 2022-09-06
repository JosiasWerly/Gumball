#pragma once
#ifndef _var
#define _var
#include <iostream>

template<class T>
class VarContainer {
public:
	T *ptr = nullptr;
	unsigned references = 1;

	VarContainer(T *ptr) :
		ptr(ptr) {
	}
	~VarContainer() {
		delete ptr;
	}
};

template<class T>
class Var {
	typedef VarContainer<T> Container;
	template<class t> friend class Var;

protected:
	Container *container = nullptr;

	Inline void unRef() {
		if (--container->references == 0) {
			delete container;
			container = nullptr;
		}
	}
	Inline void addRef(Container *newTarget) {
		setRef(newTarget);
		++container->references;
	}
	Inline void setRef(Container *newTarget) {
		container = newTarget;
	}

public:
	virtual ~Var() {
		unRef();
	}
	Var(T *newValue = nullptr) {
		setRef(new Container(newValue));
	}
	Var(const Var &other) {
		addRef(other.container);
	}
	Var(Var &&other) noexcept {
		addRef(other.container);
	}
	Var &operator=(const Var &other) {
		unRef();
		addRef(other.container);
		return *this;
	}

	T &operator*() { return *container->ptr; }
	T *operator->() { return container->ptr; }
	operator bool() { return container && container->ptr; }
	bool operator==(const Var &other) const { return container == other.container; }
	Inline unsigned references() { container->references; }
	Inline T *pin() { return container->ptr; }
	template<class t> t *pin() { return static_cast<t *>(container->ptr); }

	void free() {
		delete container->ptr;		
		container->ptr = nullptr;
	}
	void assign(T* init) {
		free();
		container->ptr = init;
	}
};

template<class T, class TStatic>
class SVar {	
	typedef Var<T> TVar;
	TVar var;
public:
	SVar() = default;
	SVar(const TVar &other) : var(other) {
	}
	SVar(TVar &&other) noexcept : var(other) {
	}
	TVar &operator=(const TVar &other) {
		return var = other;
	}

	TStatic *pin() { return var.pin<TStatic>(); }
	TStatic &operator*() { return *var.pin<TStatic>(); }
	TStatic *operator->() { return var.pin<TStatic>(); }
	operator bool() { return var; }
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