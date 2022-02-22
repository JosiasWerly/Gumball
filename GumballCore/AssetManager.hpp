#pragma once
#ifndef _assetManager
#define _assetManager

#include "Patterns.hpp"
#include "EngineSystem.hpp"
#include "Archive.hpp"
#include <list>
#include <string>
using namespace std;

/*this will become async load in some time in the future*/

class IAssetContent {
public:
	void * const dataPtr = nullptr;
	IAssetContent(void *dataPtr) : dataPtr(dataPtr) {}
};
template<class T> class TAssetContent : 
	public IAssetContent{
public:
	T *data;
	TAssetContent(T *data) : IAssetContent(data), data(data){}
	~TAssetContent() { delete data;	}
};
class Asset {
	string filePath;
	string name;
	IAssetContent *content = nullptr;
public:
	Asset(string name) : name(name) {}
	template<class T> void operator>>(T *&data) {
		data = static_cast<T*>(content->dataPtr);
	}
	template<class T> void operator<<(T *data) {
		if (content)
			delete content;
		content = new TAssetContent<T>(data);
	}
	Inline bool isValid() { return content; }
	Inline string getName() { return name; }
};


class IAssetFactory {
protected:
	list<string> extensions;
public:
	const string name;

	IAssetFactory(string name = "") : 
		name(name) {
	}
	virtual bool probeFile(const string &filePath);
	virtual bool assemble(Asset &asset, Archive &ar) = 0;
	virtual bool disassemble(Asset &asset, Archive &ar) = 0;
};
class AssetsSystem :
	public IEngineSystem {

	list<IAssetFactory*> factories;
	list<Asset*> assets;
protected:
	virtual void initialize() override;
	virtual void shutdown() override;

public:
	
	Asset *operator[](string name) {
		for (auto a : assets) {
			if (a->getName() == name)
				return a;
		}
		return nullptr;
	}
	template<class T>bool operator()(string name, T *&target) {
		if (auto asset = this->operator[](name)) {
			*asset >> target;
			return true;			
		}
		target = nullptr;
		return false;
	}
	Asset* createAsset(string name);
	void loadFile(const string& assetPath);	
	void loadAllFiles(string root);
	IAssetFactory *findFactory(const string &ext);
};

#endif // !_assetManager