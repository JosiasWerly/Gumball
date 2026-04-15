#pragma once
#ifndef __ARCHIVE
#define __ARCHIVE

#include <string>
#include <fstream>
#include <set>
#include "Gumball/Containers/Pointer.hpp"
using namespace std;

struct GENGINE FilePath {
private:
	string rpath;

public:
	FilePath() : rpath("") {}
	FilePath(const string &path) : rpath(path) {}
	string path() const;
	string name() const;
	string extension() const;
	u64 hash() const;
	bool exists() const;
	operator bool() const { return rpath != ""; }
	bool operator==(const FilePath &o) const { return rpath == o.rpath; }
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
	bool getLine(string &str);
	Inline void close();


	template<class T> Archive &operator<<(T &val);
	template<class T> Archive &operator>>(T &val);
	template<> Archive &operator>><std::string>(std::string &val);
	template<> Archive &operator<<<std::string>(std::string &val);
	template<class T> T read();
	template<class T> void write(T);

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
template<class T> T Archive::read() {
	T t;
	this->operator>><T>(t);
	return t;
}
template<class T> void Archive::write(T t) {
	this->operator<<(t);
}
template<class T> Archive &Archive::operator<<(T &val) {
	fs->operator<<(val);
	return *this;
}
template<class T> Archive &Archive::operator>>(T &val) {
	fs->operator>>(val);
	return *this;
}


class GENGINE FileSerializer {
public:
	virtual ~FileSerializer() = default;
	virtual void load(const FilePath &fp, PtrVoid &ptr) = 0;
	virtual void save(const FilePath &fp, PtrVoid &ptr) = 0;
	virtual bool supports(const FilePath &fp) const = 0;
};

template<class T>
class TFileSerializer : public FileSerializer { //TODO: I don't think we need the virtualization of this class.. CRT should fix this?
public:
	virtual ~TFileSerializer() = default;
	virtual bool load(const FilePath &fp, T &val) = 0;
	virtual bool save(const FilePath &fp, T &val) = 0;
	virtual bool supports(const FilePath &fp) const = 0;

	void load(const FilePath &fp, PtrVoid &ptr) {
		Ptr<T> tptr(new T);
		if (!load(fp, *tptr))
			~tptr;
		ptr = tptr;
	}
	void save(const FilePath &fp, PtrVoid &ptr) {
		Ptr<T> tptr = ptr;
		if (!tptr)
			return;
		save(fp, *tptr);
	}
};

template<class T>
class WFileSerializer : public TFileSerializer<T> {};

#endif // !__ARCHIVE