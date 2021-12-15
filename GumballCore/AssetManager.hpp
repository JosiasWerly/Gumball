#pragma once
#ifndef __assetManager
#define __assetManager

#include "Patterns.hpp"
#include "EngineSystem.hpp"
#include <list>
#include <string>
#include <fstream>
using namespace std;

/*this will become async load in some time in the future
*/




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

//for now this is just "adaptor" for the fstream, but in the future this will become a more autonomous class.
class Archive{
	using Long = long long;
	fstream *fs = nullptr;
public:
	Archive() {}
	Archive(string filePath) {
		open(filePath);
	}
	Archive(const Archive &other) {
		fs = other.fs;
	}
	Archive &operator=(const Archive &other) = delete;
	~Archive() {
		fs->close();
		delete fs;
	}


	void open(string filePath) {
		fs = new fstream();
		fs->open(filePath, std::fstream::in | std::fstream::out);
	}
	void close() {
		fs->close();
		delete fs;
	}
	bool getLine(string &str) {
		return (bool)std::getline(*fs, str);
	}
	void writeLine(string strg) {
		*fs << strg + '\n';		
	}

	template<class T>Archive &operator<<(T &val) {
		fs.operator<<<T>(val);
		return *this;
	}
	template<class T>Archive &operator>>(T &val) {
		fs.operator>><T>(val);
		return *this;
	}


	Inline void setg(Long newPos) { fs->seekg(newPos); }
	Inline void setp(Long newPos) { fs->seekp(newPos); }	
	Inline Long tellg() { return fs->tellg(); }
	Inline Long tellp() { return fs->tellp(); }
	Inline bool isOpen() { return fs->is_open(); }
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
	public IEngineSystem,
	public Singleton<AssetsSystem> {

	list<IAssetFactory*> factories;
	list<Asset*> assets;
protected:
	virtual void initialize() override;
	virtual void shutdown() override;

public:
	
	Asset *operator[](string name) {
		for (auto &a : assets) {
			if (a->getName() == name)
				return a;
		}
		return nullptr;
	}

	void loadAsset(const string& assetPath);	
	void loadAllAssets(string root);
	IAssetFactory *findFactory(const string &ext);
};

#endif


/*

class Asset:
	setContent(T)
	T getContent()

	bool load() {
		fac->load()
	}

class IAssetFactory:
	bool assemble(Asset*&, FArchive):
	bool disassemble(Asset*&, FArchive):

class AssetSystem:
	loadAssets(string root):
		for (it; root):
			if (fac = findFactory):
				as = new Asset
				ar = readData(it)
				if(fac->assemble(as, ar)):
					assets.add(as)
				else:
					delete as

*/