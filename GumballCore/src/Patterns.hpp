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
using namespace std;

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

template<class TValue> 
class AssetSystem {
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
    virtual const TValue* get(string name) {
        it i;
        contain(name, i);
        return (*i).second;
    }
};
#endif // !_patterns