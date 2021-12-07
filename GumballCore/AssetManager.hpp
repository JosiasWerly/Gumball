#pragma once
#ifndef __assetManager
#define __assetManager

#include "EngineSystem.hpp"
#include "Utils.hpp"
#include <list>
#include <string>
using namespace std;

class IAssetFactory {
protected:
	list<string> possibleExtensions;
public:
	const string name;

	IAssetFactory(string name = "") : 
		name(name) {
	}
	virtual bool canBuild(const string &fileExt) = 0;
	virtual bool load(const string &fileExt) = 0;
	virtual bool unload(const string &fileExt) = 0;
};

class Asset {
public:
	string filePath;
};

class AssetsSystem :
	public IEngineSystem,
	public Singleton<AssetsSystem> {

	list<IAssetFactory*> factories;
	list<Asset *> assets;
public:
	virtual void Initialize() override;
	virtual void Shutdown() override;
	
	void loadAsset(const string& assetPath);	
	void loadAllAssets(string root);
	IAssetFactory *findFactory(const string &ext);
};

#endif