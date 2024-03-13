/*
#################### SUMMARY ####################
after failed attemps of making libtool work I've decided to implement a quick real time reflection.
objectives:
-send a string and it should create the right object with properties values properly assigned
-minimal overhead to objects
-json as output and input, simple schema
-extensibility, enable its improvements as time goes by
-minimal boilerplate code, although the odds are against me
-at this version it is not optimized


Note the following classes, observe how boring and lacking in reflection they are,
because of that we cannot "save" these objects.

class Super {
public:
};

class A : public Super {
public:
	float fa, fb;
};

class B : public Super {
public:
	int a, b, c;
	string str;
	A obj;
};

the steps that you need to do:
1)Extend the classes from "Class"
2)Make class schema description
	a) using ClassTypeCtor<T> enables the declaration of single types
	b) using ClassTypePackage enables the declaration of multiple types that are considered in the same "package"
3)Enjoy the wonders os reflection

#################### ClassTypeCtor<T> Example ####################
class Super : public Class {
public:
	virtual ClassType *getType() { return Activator::instance()->get("Super"); }
};

class A : public Super {
public:
	float fa, fb;

	virtual ClassType *getType() { return Activator::instance()->get("A"); }
};
class B : public Super {
public:
	int a, b, c;
	string str;
	A obj;

	virtual ClassType *getType() { return Activator::instance()->get("B"); }
};

void main(){
	auto activator = Activator::instance();

	activator->add(
		ClassTypeCtor<A>("A")
	);
	activator->add(
		ClassTypeCtor<B>("B")
		.prop<int>("a", &B::a)
		.prop<int>("b", &B::b)
		.prop<int>("c", &B::c)
		.prop<string>("str", &B::str)
		.prop<A>("obj", &B::obj)
	);

	B bObj;
	bObj.a = 0xABCDDCBA;
	bObj.b = 0xFAFAAFAF;
	bObj.c = 0xCADAADCA;
	bObj.str = "mystring";
	bObj.obj.fa = 1.123f;
	bObj.obj.fb = -4.321f;

	string data = activator->save(&bObj);
	Class *ca = activator->load(data);
}


#################### ClassTypePackage Example ####################
struct ClassPackage_Test : public ClassTypePackage {
	string name() { return "test"; }
	std::list<ClassType *> types() {
		return {
			ClassTypeCtor<Super>("Super"),
			ClassTypeCtor<A>("A"),
		};
	}
};

void main() {
	auto activator = Activator::instance();
	activator->addPackage<ClassPackage_Test>();

	activator->add(
		ClassTypeCtor<B>("B")
		.prop<int>("a", &B::a)
		.prop<int>("b", &B::b)
		.prop<int>("c", &B::c)
		.prop<string>("str", &B::str)
		.prop<A>("obj", &B::obj)
	);

	B bObj;
	bObj.a = 0xABCDDCBA;
	bObj.b = 0xFAFAAFAF;
	bObj.c = 0xCADAADCA;
	bObj.str = "mystring";
	bObj.obj.fa = 1.123f;
	bObj.obj.fb = -4.321f;

	string data = activator->save(&bObj);
	Class *ca = activator->load(data);

	activator->delPackage<ClassPackage_Test>();
}
*/

#pragma once
#ifndef _activator
#define _activator

#include "Patterns.hpp"

#include <list>
#include <unordered_map>
#include <string>




class Class;
class ClassType;

class Property;


typedef unordered_map<string, Property *> Properties;

class Class {
public:
	virtual class ClassType *getClassType() = 0;
};

class ClassType {
	template<class TClass>	friend class ClassCtor;
	typedef void *(*FnClone)(void);

	FnClone fnClone;
	Properties properties;
	string name;

public:
	ClassType() {
	}
	virtual ~ClassType() { 
		for (auto p : properties) 
			delete p.second; 
	}
	Properties &getProperties() { return properties; }
	const string &getName() { return name; }
};

class PropertyData {
public:
	PropertyData() = default;
	virtual ~PropertyData() = default;

	virtual ClassType *getClassType() { return nullptr; }
};

template<class TType>
class TPropertyValue : public PropertyData {
	template<class TClass>	friend class ClassCtor;

	intptr_t address;
public:
	TPropertyValue() = default;
	TType *get(void *obj) { return reinterpret_cast<TType *>(reinterpret_cast<uintptr_t>(obj) + address); }
};

template<class TType>
class TPropertyClass : public TPropertyValue<TType> {
	template<class TClass>	friend class ClassCtor;

	ClassType *classType;
public:
	TPropertyClass() = default;
	ClassType *getClassType() override { return classType; }
};

class Property {
	template<class TClass>	friend class ClassCtor;

	string name;
	PropertyData *data;
public:
	Property() = default;
	virtual ~Property() = default;

	const string &getName() { return name; }

	template<class t> bool is() {
		return static_cast<bool>(dynamic_cast<TPropertyValue<t>*>(data));
	}
	template<> bool is<ClassType>() {		
		return static_cast<bool>(data->getClassType());
	}

	template<class t> t *as() = delete;
	template<> ClassType *as<ClassType>() {
		return data->getClassType();
	}
	template<class t> t *as(void *obj) {
		auto casted = dynamic_cast<TPropertyValue<t>*>(data);
		return casted ? casted->get(obj) : nullptr;
	}
};



class GBCORE Activator : public Singleton<Activator> {
private:
	typedef std::unordered_map<string, ClassType *> ClassTypes;

	ClassTypes classTypes;

public:
	void add(ClassType *classType);
	void del(const string &name);
	ClassType *get(const string className);

	Class *load(const string &data);
	string save(Class *obj);
};

template<class TClass>
class ClassCtor {
	ClassType *instance;

public:
	ClassCtor(const string &name) {
		instance = new ClassType;
		instance->fnClone = []()->void * { return new TClass; };
		instance->name = name;
	}
	~ClassCtor() {
		delete instance;
	}

	template<class TProp>
	ClassCtor<TClass> &prop(const string &name, TProp TClass:: *address) {
		Property *prop = new Property;

		TPropertyValue<TProp> *pValue = new TPropertyValue<TProp>;
		pValue->address = reinterpret_cast<intptr_t>(&(((TClass *)nullptr)->*address));
		prop->data = pValue;

		prop->name = name;
		instance->properties[name] = prop;
		return *this;
	}
	
	template<class TProp>
	ClassCtor<TClass> &prop(const string className, const string &name, TProp TClass:: *address) {
		Property *prop = new Property;

		TPropertyClass<TProp> *pClass = new TPropertyClass<TProp>;
		pClass->address = reinterpret_cast<intptr_t>(&(((TClass *)nullptr)->*address));
		pClass->classType = Activator::instance()->get(className);		
		prop->data = pClass;

		prop->name = name;
		instance->properties[name] = prop;
		return *this;
	}
	operator ClassType *() {
		ClassType *out = instance;
		instance = nullptr;
		return out;
	}
};

#include "ActivatorProperties.hpp"
#endif //_activator