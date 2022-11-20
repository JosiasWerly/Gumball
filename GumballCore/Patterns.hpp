#pragma once
#ifndef _patterns
#define _patterns

template<typename T>
class Singleton {
protected:
	static inline T *inst = nullptr;


	Singleton() {
	}
	~Singleton() {
	}
	Singleton(Singleton &other) = delete;
	void operator=(Singleton &other) = delete;
public:
	static T *setInstance(T *newInstance) {
		return inst = newInstance;
	}
	static T *instance() {
		return Singleton::inst;
	}
};

//template<class TBase>
//class SubClass {
//protected:
//	SubClass() = default;
//	~SubClass() = default;
//
//	virtual long long marker() const = 0;
//public:
//	virtual TBase *instantiate() const;
//	bool operator==(const SubClass &other) const { return marker() == other.marker(); }
//};
//
//template<class TBase, class T>
//class TSubClass :
//	public SubClass<TBase> {
//protected:
//	long long marker() const override { return static_cast<int>(&TSubClass<TBase, T>::TSubClass); }
//public:
//	virtual TBase *instantiate() const { return new T };
//};

#endif // !_patterns