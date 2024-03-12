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

template<class TType> class TProperty;

class Class {
public:
	virtual class ClassType *getClassType() = 0;
};

class Property {
public:
	Property() = default;
	virtual ~Property() = default;
	
	virtual const string serialize(Class *obj) = 0;
	virtual void serialize(Class *obj, const string &str) = 0;

	template<class t> bool is() { return static_cast<bool>(dynamic_cast<TProperty<t>*>(this)); }
	template<class t> t *as(Class *obj) { 
		if (auto casted = dynamic_cast<TProperty<t> *>(this)) {
			return casted->get(obj);
		}		
		return nullptr;
	}
};

template<class TType>
class TProperty : public Property {
private:
	intptr_t ptr;

public:
	TProperty(intptr_t ptr) : ptr(ptr) {}
	TType *get(Class *obj) { return reinterpret_cast<TType *>(reinterpret_cast<uintptr_t>(obj) + ptr); }
	const string serialize(Class *obj) override final { return std::string(reinterpret_cast<char *>(get(obj)), sizeof(TType)); }
	void serialize(Class *obj, const string &str) override final { std::memcpy(get(obj), str.data(), sizeof(TType)); }
};

typedef unordered_map<string, Property *> Properties;
class ClassType {
	template<class TClass> friend class ClassTypeCtor;
	friend class Activator;
	typedef Class *(*TFnxNew)(void);

private:
	string name;
	TFnxNew fnxNew;
	Properties properties;

public:
	ClassType() {
	}
	~ClassType() {
		for (auto kv : properties) {
			delete kv.second;
		}
		properties.clear();
	}

	const string &getName() const { return name; }
	Class *getNew() const { return fnxNew(); }
	Properties &getProperties() { return properties; }

	bool operator==(const ClassType &other) const { return getName() == other.getName(); }
	operator bool() const { return name != "" && fnxNew; }
};

struct ClassTypePackage {
	virtual string name() = 0;
	virtual std::list<ClassType*> types() = 0;
};

template<class TClass>
class ClassTypeCtor {
	ClassType *instance;

public:
	ClassTypeCtor(const string &name) {
		instance = new ClassType;
		instance->name = name;
		instance->fnxNew = []()->Class * { return new TClass; };
	}
	~ClassTypeCtor() {
		delete instance;		
	}

	template<class TProp> 
	ClassTypeCtor<TClass> &prop(const string &name, TProp TClass:: *val) {
		intptr_t ptr = reinterpret_cast<intptr_t>(&(((TClass *)nullptr)->*val));
		instance->properties[name] = new TProperty<TProp>(ptr);
		return *this;
	}
	operator ClassType *() { 
		ClassType *out = instance;
		instance = nullptr;
		return out;
	}
};

class GBCORE Activator : public Singleton<Activator> {
private:
	typedef std::list<string> Package;
	typedef std::unordered_map<string, ClassType*> Signatures;
	typedef std::unordered_map<string, Package> Packages;

	Signatures signatures;
	Packages packages;

public:	
	void add(ClassType *classType);
	void del(const string &name);
	ClassType *get(const string className);

	Class *load(const string &data);
	string save(Class *obj);

	template<class T> void addPackage() {
		ClassTypePackage *data = new T;
		const string &packageName = data->name();
		if (!packages.contains(packageName)) {
			packages.emplace(packageName, Package());
		}
		Package &package = packages[packageName];

		std::list<ClassType *> types = data->types();
		for (auto type : types) {
			add(type);
			package.push_back(type->getName());
		}
		delete data;
	}
	template<class T> void delPackage() {
		ClassTypePackage *data = new T;
		const string packageName = data->name();

		if (!packages.contains(packageName))
			return;

		Package &package = packages[packageName];
		for (auto &name : package) {
			del(name);
		}
		packages.erase(packageName);
		delete data;
	}
};

#include "ActivatorProperties.hpp"
#endif //_activator