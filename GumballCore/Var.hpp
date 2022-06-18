#pragma once
#ifndef _var
#define _var
#include <iostream>
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
	
	//HACK: i think this is wrong nor used anymore
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





//TO EVALUATE

//namespace x {
//	class Data {
//	public:
//		Data() = default;
//		virtual ~Data() {}
//	};
//	template<class T>
//	class TData :
//		public Data {
//	public:
//		T *traw;
//		TData(T *init) {
//			traw = init;
//		}
//		~TData() {
//			delete traw;
//		}
//	};
//
//	struct Target {
//		Data *data = nullptr;
//		unsigned references = 1;
//
//		Target() {
//		}
//		~Target() {
//			delete data;
//		}
//	};
//
//	template<class T>
//	class TVar {
//	protected:
//		Target *target;
//		TData<T> *tData;
//
//		inline void release() {
//			if (--target->references == 0) {
//				delete target;
//				target = nullptr;
//			}
//		}
//		inline void changeRef(Target *newTarget) {
//			release();
//			target = newTarget;
//			++target->references;
//			traw = dynamic_cast<TData<T>*>(target->data);
//		}
//	public:
//		virtual ~TVar() {
//			release();
//		}
//		TVar(T *init = nullptr) {
//			target->data = new TData<T>(init);
//		}
//		TVar(const TVar &other) {
//			target = other.target;
//			++target->references;
//		}
//		TVar(TVar &&other) {
//			target = other.target;
//			++target->references;
//		}
//		TVar &operator=(const TVar &other) {
//			changeRef(other.target);
//			return *this;
//		}
//
//		template<class t> TVar<t> convert() {
//			TVar<t> out;
//			out.target = target;
//			out.tData = dynamic_cast<TData<T>*>(target->data);
//			++target->references;
//			return out;
//		}
//
//		T *&operator->() {
//			return traw->traw;
//		}
//		inline T *&set(T *newT) {
//			delete tData->traw;
//			tData->traw = newT;
//		}
//		inline T *&get() {
//			return tData->traw;
//		}
//
//		operator bool() const {
//			return tData && tData->traw;
//		}
//		bool operator==(const TVar<T> &other) {
//
//		}
//	};
//
//	class Var :
//		public TVar<void> {
//	public:
//		using TVar::TVar;
//		Var(void *init) = delete;
//		Var() = default;
//	};
//};
//
//namespace y {
//	class Data {
//	public:
//		Data() = default;
//		virtual ~Data() {}
//	};
//	template<class T>
//	class TData :
//		public Data {
//	public:
//		T *traw;
//		TData(T *init) {
//			traw = init;
//		}
//		~TData() {
//			delete traw;
//		}
//
//	};
//
//	struct Target {
//		Data *data = nullptr;
//		unsigned references = 1;
//
//		Target() {
//			cout << this << endl;
//		}
//		~Target() {
//			cout << "~" << this << endl;
//			delete data;
//		}
//	};
//
//	class Var {
//	protected:
//		Target *target;
//
//		Inline void release() {
//			if (--target->references == 0) {
//				delete target;
//				target = nullptr;
//			}
//		}
//		Inline void changeRef(Target *newTarget) {
//			release();
//			target = newTarget;
//			++target->references;
//		}
//		Inline void setRef(Target *newTarget) {
//			target = newTarget;
//			++target->references;
//		}
//	public:
//		virtual ~Var() {
//			release();
//		}
//		Var() {
//			target = new Target();
//		}
//		Var(const Var &other) {
//			setRef(other.target);
//		}
//		Var(Var &&other) {
//			setRef(other.target);
//		}
//		Var &operator=(const Var &other) {
//			changeRef(other.target);
//			return *this;
//		}
//	};
//
//	template<class T>
//	class TVar :
//		public Var {
//	public:
//		TVar(T *init = nullptr) {
//			target->data = new TData<T>(init);
//		}
//		TVar(const TVar &other) {
//			setRef(other.target);
//		}
//		TVar(TVar &&other) {
//			setRef(other.target);
//		}
//		TVar &operator=(const TVar &other) {
//			changeRef(other.target);
//			return *this;
//		}
//	};
//};

namespace z {
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
		void* getPtrRef() { return &raw; }
	};


	struct VarTarget {
		IVarData *data = nullptr;
		unsigned references = 1;

		VarTarget(IVarData *init) : 
			data(init) {
			std::cout << int(this) << std::endl;
		}		
		~VarTarget() {
			std::cout << "~" << int(this) << std::endl;
		}
	};

	template<class T>
	class TVar {
		template<class t> friend class TVar;

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

			if (auto dCast = dynamic_cast<TVarData<T>*>(target->data))
				targetValue = &dCast->raw;
			else if (targetValue = reinterpret_cast<T **>(target->data->getPtrRef()))
				;
			else
				throw;
		}		

	public:
		virtual ~TVar() {
			release();
		}
		TVar(T *newValue = new T) {
			setRef(new VarTarget(new TVarData<T>(newValue)));
		}
		TVar(const TVar &other) {
			setRef(other.target);
			++target->references;
		}
		TVar(TVar &&other) {
			setRef(other.target);
			++target->references;
		}
		TVar &operator=(const TVar &other) {
			changeRef(other.target);			
			return *this;
		}

		T &operator*() {
			return **targetValue;
		}
		T *operator->() {
			return *targetValue;
		}
		operator bool() {
			return target && targetValue;
		}
		bool operator==(const TVar &other) const {
			return target == other.target;
		}


		Inline unsigned references() { target->references; }
		Inline T *pin() { return *targetValue; }
		template<class t> t *pin() { return dynamic_cast<t*>(*targetValue); }

		template<class t>
		TVar<t> as() {
			TVar<t> out;
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
			return out;
		}
	};
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


