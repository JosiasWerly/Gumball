#pragma once
#ifndef _assetManager
#define _assetManager

#include "Patterns.hpp"
#include "Object.hpp"
#include "Var.hpp"
#include "EngineSystem.hpp"
#include "Archive.hpp"

#include <list>
#include <string>
using namespace std;

class Asset;
class IAssetFactory;
class AssetsSystem;

/*
* TODO: 
* "minor" rework, today for retrieve an asset you need to:
* Engine::instance()->assetSystem->getAsset("logo")->getContent();
* 
* and should be
* Engine::instance()->assetSystem.getAsset<T>(Name)->getContent();
* in this manner we can avoid searching all the list of assets.
*/
class Asset {
	friend class AssetsSystem;
private:
	string filePath = "";
	string name = "";

protected:
	Var<Object> content;
public:
	Object *getContentCloned();
	Inline Var<Object>& getContent(){
		return content;
	}
	Inline void setContent(Object *newContent) {
		content.assign(newContent);
	}

	Inline bool isValid() { return content; }
	Inline const string& getName() { return name; }
	Inline const string& getPath() { return filePath; }
};
class IAssetFactory {
protected:
	list<string> extensions;
public:
	const string name;

	IAssetFactory(string name = "") : 
		name(name) {
	}
	bool hasExtension(const string &extention);
	virtual bool assemble(Object *&content, Archive &ar) = 0;
	virtual bool disassemble(Object *&content, Archive &ar) = 0;
};
class AssetsSystem :
	public IEngineSystem {

	list<IAssetFactory *> factories;
	list<Asset *> assets;

protected:
	virtual void initialize() override;
	virtual void shutdown() override;

public:
	Asset *getAsset(string name);
	template<class T> T *getAsset(string name) {
		T *target;
		if (auto asset = getAsset(name)) {
			*asset >> target;
			return target;
		}
		return nullptr;
	}


	Asset *createAsset(string name);
	void unloadAsset(string name);
	void loadAssetFromFile(const string &assetPath);
	void loadAssetsFromFolder(string root);
	void reloadAsset(string name);
	
	IAssetFactory *findFactory(const string &extension);
	void createFactory(IAssetFactory *newFactory);
	
	bool assembleObject(Object *&content, const string &assetPath);
};

#endif // !_assetManager