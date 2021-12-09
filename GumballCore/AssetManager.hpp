#pragma once
#ifndef __assetManager
#define __assetManager

#include "Patterns.hpp"
#include "EngineSystem.hpp"
#include <list>
#include <string>
using namespace std;



class IAssetContent {
public:
};
template<class T> class TAssetContent : 
	public IAssetContent{
public:
	T *Data;
	TAssetContent(T *Data) : Data(Data){}
	~TAssetContent() { delete Data;	}
};


class Asset {
	string filePath;
	IAssetContent *content = nullptr;
public:
	template<class T> void setContent(T *content) {
		if (content)
			delete content;
		content = new TAssetContent<T>(content);
	}
	template<class T> T *getContent() {
		if (TAssetContent<T> tContent = dynamic_cast<TAssetContent<T>>(content))
			return tContent.Data;
		return nullptr;
	}
	inline bool isValid() { return content; }
};
class IAssetFactory {
protected:
	list<string> extensions;
public:
	const string name;

	IAssetFactory(string name = "") : 
		name(name) {
	}
	virtual bool canBuild(const string &filePath);
	virtual bool load(const string &filePath, Asset *&Asset) = 0;
	virtual bool unload(const string &filePath, Asset *&Asset) = 0;
};

class AssetsSystem :
	public IEngineSystem,
	public Singleton<AssetsSystem> {

	list<IAssetFactory*> factories;
	list<Asset *> assets;
public:
	virtual void initialize() override;
	virtual void shutdown() override;
	
	void loadAsset(const string& assetPath);	
	void loadAllAssets(string root);
	IAssetFactory *findFactory(const string &ext);
};

#endif