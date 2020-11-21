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

#include "Gumball.hpp"
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




class AssetContent {
    struct sAssetData {
    protected:
        sAssetData() {}
    };
    template<class T> struct tAssetData :
        public sAssetData {
        tAssetData(T data) :
            data(data) {
        }
        T data;
    };
    
    sAssetData* content = nullptr;
public:
    AssetContent(){}
    template<class T>void set(T value) {
        if (content)
            delete content;
        content = new tAssetData<T>(value);
    }
    template<class T>T get() {
        return reinterpret_cast<tAssetData<T>*>(content)->data;
    }

};
class Asset {
public:
    unsigned int id;
    AssetContent content;
};

class iAssetFactory {
    friend class AssetManager;
    class AssetManager* assetManager;
public:
    virtual bool canBuild(const string& filePath) = 0;
    virtual AssetContent loadFromDisk(const string& filePath) = 0;
    virtual bool unLoad(AssetContent data) = 0;
};

class AssetManager : 
    public Singleton<AssetManager> {
public:
    map<string, Asset*> assets;
    gMap<iAssetFactory> assetFactories;

    bool loadAssetFromDisk(string filePath) {
        string fileName = gbLib::getNameOfFilePath(filePath),
               fileExt = gbLib::getExtOfFilePath(filePath);

        auto& assets = assetFactories.getAssets();
        auto it = assets.begin();
        for (; it != assets.end(); it++) {
            iAssetFactory* fact = it->second;
            if (fact->canBuild(filePath)) {
                AssetContent content = fact->loadFromDisk(fileName);
                if (content.get) {
                    Asset* newAsset = new Asset();
                    newAsset->content = content;
                    assets.emplace(fileName, newAsset);
                    return true;
                }
            }
        }
        return false;
    }


    template<class T>T* getFactory(string name) {
        return *reinterpret_cast<T*>(assetFactories.get(name));
    }
    template<>iAssetFactory* getFactory(string name) {
        return assetFactories.get(name);
    }

    template<class T>T& getAsset(string name) {
        return *reinterpret_cast<T*>(loadedAssets.get(name));
    }
};
#endif // !_patterns