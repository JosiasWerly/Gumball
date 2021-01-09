#pragma once
#ifndef _patterns
#define _patterns

#include <iostream>
#include <filesystem>
#include <set>
#include <vector>
#include <array>
#include <list>
#include <map>

#include "GFunctionLibrary.hpp"
using namespace std;

template<class T> struct Dt {
public:
	T* value = nullptr;
	unsigned int refs = 1;
	Dt(T* init) :
		value(init) {
	}
	~Dt() {
		delete value;
	}
};
template<class T> class Var {
public:
	typedef Var<T> varT;
	typedef Dt<T> dtT;

	dtT* dt;
	Var(T* ptr = nullptr) {
		dt = new dtT(ptr ? ptr : new T);
	}
	Var(varT& Other) {
		dt = Other.dt;
		dt->refs++;
	}
	~Var() {
		if (dt && --dt->refs == 0) {
			delete dt;
		}
	}

	varT& operator=(varT& cpy) {
		if (dt != cpy.dt) {
			if (dt)
				dt->refs--;
			dt = cpy.dt;
			dt->refs++;
		}
		return *this;
	}

	operator T* () {
		return dt ? dt->value : nullptr;
	}
	T* operator=(T* newValue) {
		if (dt->value)
			delete dt->value;
		dt->value = newValue;
		return dt->value;
	}
	T* operator()() {
		return dt->value;
	}

	operator bool() {
		return dt->value;
	}
};

template<typename T>
class Singleton {
protected:
	Singleton() {
	}
	~Singleton() {
	}
	Singleton(Singleton& other) = delete;
	void operator=(Singleton& other) = delete;
public:
	static T& instance() {
		static T instance;
		return instance;
	}
};

template<typename TValue>
class gMap {
public:
	typedef pair<string, TValue*> pAsset;
	typename typedef map<string, TValue*>::iterator it;
protected:
	map<string, TValue*> assets;
public:
	map<string, TValue*>& getAssets() {
		return assets;
	}
	bool contain(string name) {
		return assets.find(name) != assets.end();
	}
	bool contain(string name, it& out) {
		out = assets.find(name);
		return  out != assets.end();
	}

	TValue* push(string name, const TValue&& init) {
		if (!contain(name)) {
			TValue* newVal = new TValue(init);
			assets.insert({ name, newVal });
			return newVal;
		}
		return nullptr;
	}
	TValue* push(string name, TValue* newVal) {
		if (!contain(name)) {
			assets.insert({ name, newVal });
			return newVal;
		}
		return nullptr;
	}
	template<class T>T* push(string name, const T&& init) {
		if (!contain(name)) {
			T* newVal = new T(init);
			assets[name] = reinterpret_cast<TValue*>(newVal);
			//assets.insert({ name, init });
			return newVal;
		}
		return nullptr;
	}

	virtual bool pop(string name) {
		it i;
		if (contain(name, i)) {
			assets.erase(i);
			return true;
		}
		return false;
	}
	virtual void clear() {
		for (pAsset i : assets)
			delete i.second;
		assets.clear();
	}
	virtual TValue* get(string name) {
		it i;
		contain(name, i);
		return (*i).second;
	}
};

///Thread
#include <thread>
class Thread{	
	class BaseBinder {
	public:
		virtual void call() = 0;
	};
	template<typename TObj>
	class Method :
		public BaseBinder {
	public:
		typedef void(TObj::*Fnx)(void);
		TObj* ob;
		Fnx fn;
		Method(TObj* obj, Fnx fnx) {
			ob = obj;
			fn = fnx;
		}
		void call() {
			(*ob.*fn)();
		}
	};
	class Function :
		public BaseBinder {
	public:
		typedef void(*Fnx)(void);
		Fnx fnx;
		
		Function(Fnx fn) {
			fnx = fn;
		}
		void call() {
			fnx();
		}
	};
	
	std::thread* th;
	BaseBinder* binder;
public:
	Thread() {
		stop();
		if (binder)
			delete binder;
	}
	Thread(void(*fnx)(void)) {
		bind(fnx);
	}
	template<typename TObj>Thread(TObj* obj, void(TObj::*fnx)(void)) {
		bind<TObj>(obj, fnx);
	}

	void bind(void(*fnx)(void)) {
		if (binder)
			delete binder;
		binder = new Function(fnx);
	}
	template<typename TObj>void bind(TObj* obj, void(TObj::*fnx)(void)) {
		if (binder)
			delete binder;
		binder = new Method<TObj>(obj, fnx);
	}
	void start() {
		stop();
		if(binder)
			th = new thread(&BaseBinder::call, binder);
	}
	void stop() {
		if (th) {
			th->join();
			delete th;
			th = nullptr;
		}
	}

	bool isBinded() {
		return binder;
	}
	bool isRunning() {
		return th;
	}
};
#endif // !_patterns