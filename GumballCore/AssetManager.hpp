#pragma once
#ifndef __assetManager
#define __assetManager

#include "Patterns.hpp"
#include "EngineSystem.hpp"
#include <list>
#include <string>
#include <fstream>
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

class Archive{
	using Long = long long;
	fstream *fs = nullptr;
	string fsPath = "";
public:
	Archive() {}
	Archive(string filePath) {
		open(filePath);
	}
	Archive(const char* filePath) {
		open(filePath);
	}
	Archive(const Archive &other) {
		fs = other.fs;
	}
	virtual ~Archive() {
		close();
	}
	Archive &operator=(const Archive &other) = delete;
	Archive &operator=(Archive other) = delete;

	void open(string filePath) {
		if (fs) {
			fs->close();
			delete fs;
		}

		fs = new fstream();
		fs->open(filePath, std::fstream::in | std::fstream::out);
		
		if (fs->is_open())
			fsPath = filePath;
		else
			delete fs;
	}
	void writeLine(string strg) {
		*fs << strg + '\n';		
	}
	Inline void close() {
		if (fs) {
			fs->close();
			delete fs;
			fs = nullptr;
			fsPath = "";
		}
	}
	bool getLine(string &str) {
		return (bool)std::getline(*fs, str);
	}

	Inline void setg(Long newPos) const { fs->seekg(newPos); }
	Inline void setp(Long newPos) const { fs->seekp(newPos); }
	Inline Long tellg() const { return fs->tellg(); }
	Inline Long tellp() const { return fs->tellp(); }
	Inline bool isOpen() const { return fs->is_open(); }
	Inline const string& filePath() { return fsPath; }
	
	template<class T>Archive &operator<<(T &val) {
		fs.operator<<<T>(val);
		return *this;
	}
	template<class T>Archive &operator>>(T &val) {
		fs.operator>><T>(val);
		return *this;
	}
	
	Archive &operator=(const string filePath) {
		open(filePath);
	}
	bool operator==(const Archive &other) const {
		return (fs && fs == other.fs);
	}
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

#endif