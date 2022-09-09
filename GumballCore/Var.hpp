#pragma once
#ifndef _var
#define _var
#include <iostream>

class IVarTarget {
public:
	virtual ~IVarTarget() {}
};
template<class T> class TVarTarget : 
	public IVarTarget {
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
	IVarTarget *var = nullptr;

	VarContainer(IVarTarget *var) : 
		var(var) {
	}
	~VarContainer() {
		delete var;
	}
};



template<class T> class Var {
	typedef VarContainer Container;
	template<class t> friend class Var;

protected:
	Container *container = nullptr;
	T *&ptr;

	Inline void unRef() {
		if (--container->references == 0) {
			delete container;
			container = nullptr;
		}
	}
	Inline void addRef(const Var &newTarget) {
		setRef(newTarget);
		++container->references;
	}
	Inline void setRef(const Var &newTarget) {
		container = newTarget.container;
		ptr = newTarget.ptr;
	}
	Inline T *&getRefTracker() {
		if (auto trg = dynamic_cast<TVarTarget<T>*>(container->var))
			return trg->ptr;
		else
			throw;
	}
	

	Var(Container *container, long ptrAddress) :
		container(container),
		ptr((T*&)ptrAddress) {
	}
public:
	virtual ~Var() {
		unRef();
	}
	Var(T *newValue = nullptr) : 
		container(new Container(new TVarTarget<T>(newValue))),
		ptr(dynamic_cast<TVarTarget<T>*>(container->var)->ptr) {
	}
	Var(const Var &other) : 
		ptr(other.ptr) {
		addRef(other);
	}
	Var(Var &&other) noexcept : 
		ptr(other.ptr) {
		addRef(other);
	}
	Var &operator=(const Var &other) {
		unRef();
		addRef(other);
		return *this;
	}

	T &operator*() { return *ptr; }
	T *&operator->() { return ptr; }
	const bool isValid() { return *this; }
	operator bool() { return container && container->var && ptr; }
	bool operator==(const Var &other) const { return container == other.container; }
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




//class IVarContainer {
//protected:
//	//IVarContainer(); TODO
//public:
//	unsigned references = 1;
//
//	virtual ~IVarContainer() {}
//};
//
//template<class T> class VarContainer : 
//	public IVarContainer {
//public:
//	T *ptr = nullptr;
//
//	VarContainer(T *ptr) :
//		ptr(ptr) {
//	}
//	~VarContainer() {
//		delete ptr;
//	}
//};
//
//class IVar {
//	typedef IVarContainer Container;
//protected:
//	Container *container = nullptr;
//
//	Inline void unRef() {
//		if (--container->references == 0) {
//			delete container;
//			container = nullptr;
//		}
//	}
//	Inline void addRef(Container *newTarget) {
//		setRef(newTarget);
//		++container->references;
//	}
//	Inline void setRef(Container *newTarget) {
//		container = newTarget;
//	}
//
//public:
//	virtual ~IVar() {
//		unRef();
//	}
//	IVar(const IVar &other) {
//		addRef(other.container);
//	}
//	IVar(IVar &&other) noexcept {
//		addRef(other.container);
//	}
//	IVar &operator=(const IVar &other) {
//		unRef();
//		addRef(other.container);
//		return *this;
//	}
//};
//
//template<class T> class Var :
//	public IVar {
//	typedef VarContainer<T> TContainer;
//	template<class t> friend class Var;
//
//	TContainer *tContainer;
//public:
//	Var(T *newValue = nullptr) {
//		setRef(new TContainer(newValue));
//	}
//	Var(const Var &other) {
//		addRef(other.container);
//	}
//	Var(Var &&other) noexcept {
//		addRef(other.container);
//	}
//	Var &operator=(const Var &other) {
//		unRef();
//		addRef(other.container);
//		return *this;
//	}
//
//	T &operator*() { return *tContainer->ptr; }
//	T *operator->() { return tContainer->ptr; }
//	operator bool() { return tContainer && tContainer->ptr; }
//	bool operator==(const Var &other) const { return tContainer == other.tContainer; }
//	Inline unsigned references() { tContainer->references; }
//	Inline T *pin() { return tContainer->ptr; }
//	template<class t> t *pin() { return static_cast<t *>(tContainer->ptr); }
//
//	void free() {
//		delete tContainer->ptr;
//		tContainer->ptr = nullptr;
//	}
//	void assign(T* init) {
//		free();
//		tContainer->ptr = init;
//	}
//	template<class t> Var<t> as() {
//		Var<t> out;
//		
//	}
//
//};
//
//template<class T, class TStatic> class SVar {	
//	typedef Var<T> TVar;
//	TVar var;
//public:
//	SVar() = default;
//	SVar(const TVar &other) : var(other) {
//	}
//	SVar(TVar &&other) noexcept : var(other) {
//	}
//	TVar &operator=(const TVar &other) {
//		return var = other;
//	}
//
//	TStatic *pin() { return var.pin<TStatic>(); }
//	TStatic &operator*() { return *var.pin<TStatic>(); }
//	TStatic *operator->() { return var.pin<TStatic>(); }
//	operator bool() { return var; }
//};
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