#pragma once
#ifndef _archive
#define _archive

#include <string>
#include <fstream>
#include "Var.hpp"
using namespace std;

namespace Path {
	string GENGINE fileName(string path);
	string GENGINE extention(string path);
};

struct FilePath {
	string path = "";

	Inline const string &getPath() const { return path; }
	Inline const string getName() const { return Path::fileName(path); }
	Inline const string getExtension() const { return Path::extention(path); }
	operator bool() const { return path != ""; }
};

class GENGINE Archive {
	using Long = long long;
	fstream *fs = nullptr;
	FilePath filePath;

public:
	Archive() = default;
	Archive(const Archive &other) = delete;
	Archive(string filePath);
	Archive(const char *filePath);
	virtual ~Archive();

	void open(string filePath);
	void writeLine(string strg);
	Inline void close();
	bool getLine(string &str);


	template<class T>Archive &operator<<(T &val);
	template<class T>Archive &operator>>(T &val);

	Archive &operator=(const Archive &other) = delete;
	Archive &operator=(Archive other) = delete;
	Archive &operator=(const string filePath);
	bool operator==(const Archive &other) const;
	
	Inline void setg(Long newPos) const { fs->seekg(newPos); }
	Inline void setp(Long newPos) const { fs->seekp(newPos); }
	Inline Long tellg() const { return fs->tellg(); }
	Inline Long tellp() const { return fs->tellp(); }
	Inline bool isOpen() const { return fs->is_open(); }
	Inline const FilePath &getFilePath() const { return filePath; }
};


class GMODULE Serializer {
public:
	virtual bool load(Archive &ar, Var &var) = 0;
	virtual bool save(Archive &ar, const Var &var) = 0;
};

template<class T>
class GMODULE TSerializer : public Serializer {
	virtual bool load(Archive &ar, T &var) = 0;
	virtual bool save(Archive &ar, const T &var) = 0;

	bool load(Archive &ar, Var &var) {
		//TVar<T> newValue();
		//var = newValue;
		//return load(ar, newValue.pin());
		return false;
	}
	bool save(Archive &ar, const Var &var) {
		return false;//save(ar, var);
	}
public:
	TSerializer() = default;
};




//template<class T> 
//class GMODULE TArchiveSerializer : public ArchiveSerializer {
//protected:
//	bool loadObj(Archive &ar, T &obj) = delete;
//	bool saveObj(Archive &ar, T &obj) = delete;
//
//public:
//	bool load(Archive &ar, Var *&var) {
//		if (var) {
//			delete var;
//			var = nullptr;
//		}
//		TVar<T> *newVar = new TVar<T>(new T);
//		if (!loadObj(ar, *newVar)) {
//			delete newVar;
//			newVar = nullptr;
//		}
//		var = newVar;
//		return newVar;
//	}
//	bool save(Archive &ar, Var &var) {
//		TVar<T> *newVar = static_cast<TVar<T>*>(&var);
//		if (!newVar || !newVar->isValid())
//			return false;
//		return saveObj(ar, *newVar);
//	}
//};

#endif // !_archive