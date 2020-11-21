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
    template<class T>T* push(string name, const T&& init) {
        if (!contain(name)) {
            T* newVal = new T(init);
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
    virtual TValue* get(string name) {
        it i;
        contain(name, i);
        return (*i).second;
    }
};


class iAsset {
public:
};

class iAssetFactory {
    friend class AssetManager;
    class AssetManager* assetManager;
public:
    virtual iAsset* loadFromDisk(string filePath) = 0;
    virtual iAsset* unLoad(void* data) = 0;
};

class AssetManager : 
    public Singleton<AssetManager> {
public:
    gMap<iAsset> loadedAssets;
    gMap<iAssetFactory> assetFactories;

    template<class T>T* getFactory(string name) {
        return *reinterpret_cast<T*>(assetFactories.get(name));
    }
    template<>iAssetFactory* getFactory(string name) {
        return assetFactories.get(name);
    }

    template<class T>T& getAsset(string name) {
        return *reinterpret_cast<T*>(loadedAssets.get(name));
    }
    template<>iAsset& getAsset(string name) {
        return *loadedAssets.get(name);
    }
};
#endif // !_patterns