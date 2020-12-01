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

#include "GFunctionLibrary.hpp"
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

//this is how we do a good decorator. but i feel there is too many layers... hate burocracy
class AssetContent {
	class sAssetData {
	protected:
		sAssetData() {}
	};
	template<class T> class tAssetData :
		public sAssetData {
	public:
		tAssetData() {}
		tAssetData(T data) :
			data(data) {
		}
		T data;
	};
	sAssetData* content = nullptr;
public:
	AssetContent() {
	}
	template<class T>AssetContent(T value) {
		this->set<T>(value);
	}
	bool isValid() {
		return content;
	}
	void free() {
		if (content)
			delete content;
	}
	template<class T>T& anew() {
		free();
		tAssetData<T>* newContent = new tAssetData<T>();
		content = newContent;
		return newContent->data;
	}
	template<class T>T& get() {
		return reinterpret_cast<tAssetData<T>*>(content)->data;
	}
};
class Asset {
public:
	unsigned int id;
	AssetContent content;
};
class iAssetFactory {
public:
	virtual bool canBuild(const string& filePath) = 0;
	virtual bool loadFromDisk(const string& filePath, AssetContent& content) = 0;
	virtual bool unLoad(AssetContent& content) = 0;
};

class AssetManager :
	public Singleton<AssetManager> {
public:
	map<string, Asset*> assets;
	map<string, iAssetFactory*> assetFactories;
	iAssetFactory* findFactoryForAsset(string filePath) {
		auto it = assetFactories.begin();
		for (; it != assetFactories.end(); it++) {
			if (it->second->canBuild(filePath))
				return it->second;
		}
		return nullptr;
	}
	void pushFactory(string name, iAssetFactory* factory) {
		assetFactories[name] = factory;
	}
	void popFactory(string name) {
		if (assetFactories.count(name) > 0) {
			delete assetFactories[name];
			assetFactories.erase(name);
		}
	}

	bool loadAssetFromDisk(string filePath) {
		string fileName = getNameOfFilePath(filePath),
			fileExt = getExtOfFilePath(filePath);
		cout << filePath;
		iAssetFactory* fact = findFactoryForAsset(filePath);
		if (fact) {
			Asset* newAsset = new Asset();
			if (fact->loadFromDisk(filePath, newAsset->content)) {
				assets.emplace(fileName, newAsset);
				cout << " OK" << endl;
				return true;
			}
			else
				delete newAsset;
		}
		cout << " BAD" << endl;
		return false;
	}

	Asset* getAsset(string assetName) {
		if (assets.count(assetName) > 0)
			return assets[assetName];
		return nullptr;
	}
	template<typename T>T& getData(string assetName) {
		return assets[assetName]->content.get<T>();
	}
};
#endif // !_patterns