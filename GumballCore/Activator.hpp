#pragma once
#ifndef _activator
#define _activator

#include "Patterns.hpp"
#include <set>
#include <unordered_map>

//TODO: figured out libtool to properly do this
//the cheapest "reflection" one can think of

class TypeWrapper {
public:
	virtual void *getNew() const = 0;
	virtual const char *getName() const = 0;
};

template<class T>
class TTypeWrapper : public TypeWrapper {
private:
	const char *name;

public:
	using Type = T;

	TTypeWrapper(const char *name) : name(name) {}
	void *getNew() const { return new T; }
	const char *getName() const { return name; }
};

struct ActivatorPackage {
	std::set<TypeWrapper *> signatures;
};

class GBCORE Activator : public Singleton<Activator> {
private:
	typedef std::set<TypeWrapper *> Package;
	std::unordered_map<const char *, Package> packages;

	TypeWrapper* packageFind(Package &package, const char *name);
public:

	void add(const char *packageName, ActivatorPackage package);
	void del(const char *packageName);
	TypeWrapper *getAt(const char *packageName, const char *name);
	TypeWrapper *get(const char *name);
};

#define addType(type) new TTypeWrapper<type>(#type)
#define getType(type) Activator::instance()->get(#type)
#endif //_activator