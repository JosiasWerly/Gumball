#pragma once
#ifndef _activatorproperties
#define _activatorproperties

#include "Activator.hpp"

//template<class TClass>
//class TProperty<TClass, string> : public Property {
//	typename typedef string TProp;
//
//private:
//	TProp TClass:: *ptr;
//
//public:
//	TProperty(TProp TClass:: *ptr) : ptr(ptr) {}
//	void set(TClass *obj, TProp propValue) {
//		obj->*ptr = propValue;
//	}
//	TProp &get(TClass *obj) {
//		return obj->*ptr;
//	}
//
//	const string serialize(Class *obj) override final {
//		return (static_cast<TClass *>(obj)->*ptr);
//	}
//	void serialize(Class *obj, const string &str) override final {
//		(static_cast<TClass *>(obj)->*ptr) = str;
//	}
//};


#endif //_activatorproperties