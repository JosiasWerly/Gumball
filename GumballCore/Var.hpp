#pragma once
#ifndef _var
#define _var

struct PContainer {
	unsigned int refCounter = 1;
	void *ptr;
	PContainer(void *ptr = nullptr) : 
		ptr(ptr) {
	}
};
template<class T>
class Var {
	template<class t> friend class Var;

	PContainer *container;
	T **tptr;
protected:
	inline void release() {
		if (--container->refCounter == 0) {
			delete *tptr;
			delete container;
			*tptr = nullptr;
			container = nullptr;
		}
	}
	inline void changeRef(PContainer *newContainer) {
		release();
		container = newContainer;
		tptr = reinterpret_cast<T **>(&container->ptr);
		container->refCounter++;
	}

public:
	~Var() {
		release();
	}
	Var(T *init = nullptr) :
		container(new PContainer(init)),
		tptr(reinterpret_cast<T **>(&container->ptr)) {
	}
	Var(const Var &other) :
		container(other.container),
		tptr(reinterpret_cast<T **>(&container->ptr)) {
		++container->refCounter;
	}
	Var(Var &&other) : 
		container(other.container),
		tptr(reinterpret_cast<T **>(&container->ptr)) {
		++container->refCounter;
	}
	Var &operator=(const Var &other) {
		changeRef(other.container);
		return *this;
	}
	
	//HACK: i think this is wrong or not used anymore
	template<class t> Var(const Var<t> &other) {
		changeRef(other.container);
	}

	inline void set(T *newVal) {
		free();
		*tptr = newVal;
	}
	inline void free() {
		delete *tptr;
		*tptr = nullptr;
	}
	inline T *pin() { 
		return *tptr; 
	}
	template<class t> t* pin() {
		return dynamic_cast<t*>(*tptr);
	}



	T *&operator*() { return *tptr; }
	T *&operator->() { return *tptr; }
	operator bool() const { return *tptr; }
	bool operator==(const Var &other) const { return container == other.container; }
	unsigned int referenceCount() const { return container->refCounter; }

	template<class t> operator Var<t>() {
		Var<t> out;
		out.changeRef(container);
		return out;
	}
};
#endif // !_var


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