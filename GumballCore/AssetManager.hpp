#pragma once
#ifndef _assetmanager
#define _assetmanager

#include "Object.hpp"
#include "Var.hpp"
#include "Subsystem.hpp"
#include "Archive.hpp"


#include <list>
#include <string>
using namespace std;

class Asset;
class AssetFactory;
class AssetsSystem;

namespace Path {
	static string extention(string path);
	static string fileName(string path);
};


class Asset {
	friend class AssetsSystem;
private:
	string filePath = "";
	string name = "";

protected:
	Var<Object> content;
public:

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

struct SubClass {
public:
	virtual Object *instantiate() const = 0;
};
template<class T>
struct TSubClass : 
	public SubClass{
public:
	Object *instantiate() const override { return new T; }
};

//this can be a tuple inside of assetSystem... no need for this class
class AssetFactory {
protected:
	SubClass *subClass = nullptr;
	list<string> extensions;
public:
	AssetFactory(SubClass *subClass, list<string> extensions);
	bool hasExtension(const string &extention);
	Object *construct() { return subClass->instantiate(); }
};	

class AssetsSystem :
	public EngineSystem {
private:
	list<AssetFactory> factories;
	list<Asset *> assets;

	AssetFactory *findFactory(const string &extension);
protected:
	virtual void initialize() override;
	virtual void shutdown() override;

public:
	void addAsset(Asset *asset);
	void delAsset(Asset *asset);	
	Asset *getAsset(string name);
	template<class T> T *getAsset(string name) {
		T *target;
		if (auto asset = getAsset(name)) {
			*asset >> target;
			return target;
		}
		return nullptr;
	}

	bool save(Asset *asset);
	bool load(Asset *asset);

	void loadFromFolder(string root);
	void loadAssetFromFile(const string &assetPath);

	ESystemTickType tickType() override { return ESystemTickType::disable; }
};

#endif // !_assetmanager