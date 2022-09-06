#pragma once
#ifndef _var
#define _var
#include <iostream>


class IVarData {
public:
	IVarData() = default;
	virtual ~IVarData() = default;
	virtual void *getPtrRef() = 0;
};

template<class T>
class TVarData :
	public IVarData {
public:
	T *raw;
	TVarData(T *init) {
		raw = init;
	}
	~TVarData() {
		delete raw;
	}
	void *getPtrRef() { return &raw; }
};

struct VarTarget {
	IVarData *data = nullptr;
	unsigned references = 1;

	VarTarget(IVarData *init) :
		data(init) {
	}
	~VarTarget() {
	}
};

template<class T>
class Var {
	template<class t> friend class Var;

protected:
	VarTarget *target = nullptr;
	T **targetValue = nullptr;

	Inline void release() {
		if (--target->references == 0) {
			delete target->data;
			delete target;
			target = nullptr;
		}
	}
	Inline void changeRef(VarTarget *newTarget) {
		release();
		setRef(newTarget);
		++target->references;
	}
	Inline void setRef(VarTarget *newTarget) {
		target = newTarget;
		if (auto castTarget = dynamic_cast<TVarData<T>*>(target->data))
			targetValue = &castTarget->raw;
		//else
		//	throw; //impossible cast
	}

public:
	virtual ~Var() {
		release();
	}
	Var(T *newValue = nullptr) {
		setRef(new VarTarget(new TVarData<T>(newValue)));
	}
	Var(const Var &other) {
		setRef(other.target);
		++target->references;
	}
	Var(Var &&other) {
		setRef(other.target);
		++target->references;
	}
	Var &operator=(const Var &other) {
		changeRef(other.target);
		return *this;
	}

	T &operator*() { return **targetValue; }
	T *operator->() { return *targetValue; }
	operator bool() { return target && targetValue; }
	bool operator==(const Var &other) const { return target == other.target; }
	Inline unsigned references() { target->references; }
	Inline T *pin() { return *targetValue; }
	template<class t> t *pin() { return dynamic_cast<t *>(*targetValue); }
	
	void free() {
		delete target->data;
		target->data = nullptr;
	}
	void assign(T *init) {
		free();
		target->data = new TVarData<T>(init);
		setRef(target);
	}

	template<class t>
	Var<t> as() {
		Var<t> out;
		out.release();
		out.target = target;
		++target->references;
		targetValue = 
		//if (auto castTarget = dynamic_cast<TVarData<T>*>(target->data)) {
		//	t *test = static_cast<t *>(castTarget->raw);
		//	int *&a = new int(2);
		//}
		return out;


		/*Var<t> out;
		out.release();
		out.target = this->target;
		++out.target->references;
		if (auto castedTarget = dynamic_cast<TVarData<T>*>(target->data)) {
			out.targetValue = reinterpret_cast<t **>(&castedTarget->raw);
			if (!out.targetValue)
				throw;
		}
		else
			throw;
		return out;*/
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