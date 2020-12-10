#pragma once
#ifndef _assetManger
#define _assetManger
#include <iostream>
#include <filesystem>
#include <set>
#include <vector>
#include <array>
#include <list>
#include <map>
#include "Patterns.hpp"
#include "GFunctionLibrary.hpp"


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
		string 
			fileName = getNameOfFilePath(filePath),
			fileExt = getExtOfFilePath(filePath);

		iAssetFactory* fact = findFactoryForAsset(filePath);
		if (fact) {
			cout << filePath;
			Asset* newAsset = new Asset();
			if (fact->loadFromDisk(filePath, newAsset->content)) {
				assets.emplace(fileName, newAsset);
				cout << " OK" << endl;
				return true;
			}
			else {
				delete newAsset;
				cout << " BAD" << endl;
				return false;

			}
		}
		return false;
	}
	void loadAssets(string rootFolderPath);

	Asset* getAsset(string assetName) {
		if (assets.count(assetName) > 0)
			return assets[assetName];
		return nullptr;
	}
	template<typename T>T& getData(string assetName) {
		return assets[assetName]->content.get<T>();
	}
};
#endif // !_assetManger
