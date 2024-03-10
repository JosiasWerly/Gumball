#pragma once
#ifndef _activator
#define _activator

#include "Patterns.hpp"
#include "Archive.hpp"

#include <set>
#include <map>
#include <unordered_map>
#include <string>



//TODO: figured out libtool to properly do this
//the cheapest "reflection" one can think of

class ClassType;

class Class {
public:
	virtual ClassType *getType() { return nullptr; }
};

class Property {
public:
	Property() = default;
	virtual ~Property() = default;

	virtual const string save(void *obj) = 0;
	virtual void load(void *obj, const string &str) = 0;
};

template<class TClass, class TProp>
class TProperty : public Property{
private:
	TProp TClass::*ptr;

public:
	TProperty(TProp TClass::*ptr) : ptr(ptr) {}
	void set(TClass *obj, TProp propValue) {
		obj->*ptr = propValue;
	}
	TProp &get(TClass *obj) {
		return obj->*ptr;
	}

	const string save(void *obj) override final {
		std::string out;
		char *ptrIt = reinterpret_cast<char *>(&(static_cast<TClass *>(obj)->*ptr));
		out.assign(ptrIt, sizeof(TProp));
		return out;
	}
	void load(void *obj, const string &str) override final {
		if (str.size() != sizeof(TProp)) {
			return; //Corrupt data, size mismatch
		}
		TProp *itPtr = &(static_cast<TClass *>(obj)->*ptr);
		std::memcpy(itPtr, str.data(), sizeof(TProp));
	}
};

typedef unordered_map<string, Property *> Porperties;
class ClassType {
	template<class T> friend class ClassTypeBuilder;
	template<class T> friend class PropertyBuilder;

private:
	string name;
	Class *(*newSignature)(void);
	Porperties properties;

public:
	ClassType() = default;
	~ClassType() = default;
	string getName() const { return name; }
	Class *getNew() const { return newSignature(); }
	Porperties &getProperties() { return properties; }

	bool operator==(const ClassType &other) const { return getName() == other.getName(); }
	bool operator<(const ClassType &other)const { return this < &other; }
};

template<class TClass>
class PropertyBuilder {
	ClassType &instance;
public:

	PropertyBuilder(ClassType &instance) : instance(instance) {}

	template<class TProp> 
	PropertyBuilder<TClass>& add(const string &name, TProp TClass::*val) {
		instance.properties[name] = new TProperty<TClass, TProp>(val);
		return *this;
	}

	operator ClassType() { return instance; }
};

template<class TClass>
class ClassTypeBuilder {
private:
	ClassType instance;
	PropertyBuilder<TClass> propertyBuilder;
public:
	ClassTypeBuilder() : propertyBuilder(instance) {}
	ClassTypeBuilder &signature(string name) {
		instance.name = name;
		instance.newSignature = []()->Class * { return new TClass; };
		return *this;
	}
	PropertyBuilder<TClass> &properties() { 
		return propertyBuilder; 
	}
	operator ClassType() { 
		return instance; 
	}
};

class GBCORE Activator : public Singleton<Activator> {
private:
	typedef std::set<ClassType> Signature;
	Signature signatures;

public:
	void add(ClassType builder);
	ClassType *get(const string name);
	Class *load(const string &data);
	string save(Class *obj);
};

#endif //_activator