#pragma once
#ifndef _assetmanager
#define _assetmanager

#include "Object.hpp"
#include "Var.hpp"
#include "Subsystem.hpp"
#include "Archive.hpp"


#include <list>
#include <set>
#include <string>
using namespace std;

class Asset;
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
	IVar *content = nullptr;

public:
	Inline IVar *getContent() { return content; }
	Inline bool isValid() { return content != nullptr; }
	Inline const string& getName() { return name; }
	Inline const string& getPath() { return filePath; }
};

class IAssetFactory {
protected:
	set<string> extensions;
public:
	IAssetFactory() {}
	bool hasExtension(const string &extention) const { return extensions.contains(extention); }
	virtual bool archiveLoad(Archive &ar, IVar *&var) = 0;
	virtual bool archiveSave(Archive &ar, const IVar *var) = 0;
};
template<class T> class TAssetFactory : public IAssetFactory {
public:
	bool archiveLoad(Archive &ar, IVar *&var) override final {
		delete var;
		T *trg = new T;
		if (load(ar, *trg)) {
			var = new Var<T>(trg);
			return true;
		}
		delete trg;
		return false;
	}
	bool archiveSave(Archive &ar, const IVar *var) override final {
		throw;
		return false;
	}

	virtual bool load(Archive &ar, T &val) = 0;
	virtual bool save(Archive &ar, const T &val) = 0;
};
template<class T> class AssetFactory : public TAssetFactory<T> {};

class AssetsSystem :
	public EngineSystem {
private:
	list<IAssetFactory*> factories;
	list<Asset *> assets;

	IAssetFactory *findFactory(const string &extension);
protected:
	virtual void initialize() override;
	virtual void shutdown() override;

public:
	void addAsset(Asset *asset);
	void delAsset(Asset *asset);	
	Asset *getAsset(string name);
	template<class t> t *getContent(string name) {
		if (auto asset = getAsset(name)) {
			if (auto var = dynamic_cast<Var<t>*>(asset->getContent())) {
				return var->pin();
			}
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