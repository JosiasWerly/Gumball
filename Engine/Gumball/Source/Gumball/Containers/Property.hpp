#pragma once
#ifndef __property
#define __property

#include "Flow/Dispatcher.hpp"

template<class TOwner, class TObj>
class TProperty {
protected:
	const TOwner *owner;
	TObj value;

public:
	TDelegate<void(const TOwner *owner, const TObj &oldValue, const TObj &value)> notify;

	explicit TProperty(TOwner *owner) : owner(owner) {}
	explicit TProperty(TOwner *owner, const TObj &&init) : owner(owner), value(init) {}
	TProperty(const TProperty &) = delete;

	Inline void operator()(TObj value) {
		if (this->value == value)
			return;
		TObj oldValue = this->value;
		this->value = value;
		notify.invoke(owner, oldValue, this->value);
	}
	Inline const TObj &operator()() const { return value; }
	Inline TObj &operator()() { return value; }
};

template<class TOwner, class TObj>
class TPropertyArr {
	using TArr = std::list<TObj*>;
	
	const TOwner *owner;	
	TArr arr;

public:
	explicit TPropertyArr(TOwner *owner) : owner(owner) {}
	~TPropertyArr() {
		for (auto i : arr)
			delete i;
	}

	TPropertyArr &operator<<(TObj *prop) {
		arr.push_back(prop);
		return *this;
	}
	TPropertyArr &operator>>(TObj *prop) {
		arr.remove(prop);
		return *this;
	}
	TPropertyArr &operator<<(TArr props) {
		for (auto i : props)
			(*this) << i;
		return *this;
	}
	TPropertyArr &operator>>(TArr props) {
		for (auto i : props)
			(*this) >> i;
		return *this;
	}
	Inline const TObj *getOwner() const { return owner; }
	Inline TArr &getArr() { return arr; };
	Inline bool hasItem(TObj *item) const { return std::find(arr.begin(), arr.end(), item) != std::end(arr); };
};

//template<class T>
//class TProperty<void, T> {
//protected:
//	T value;
//
//public:
//	Delegate<void(const T &oldValue, const T &value)> notify;
//
//	explicit TProperty(const T &&init = T()) : value(value) {}
//	TProperty(const TProperty &) = delete;
//
//	Inline void operator()(T value) {
//		if (this->value == value)
//			return;
//		T oldValue = this->value;
//		this->value = value;
//		notify.invoke(oldValue, this->value);
//	}
//	Inline const T &operator()() const { return value; }
//	Inline T &operator()() { return value; }
//};


#endif // !__property