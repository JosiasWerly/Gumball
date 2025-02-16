#pragma once
#ifndef _archive
#define _archive

#include <string>
#include <fstream>
#include <set>
#include "Pointer.hpp"
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
	Inline void close();
	bool getLine(string &str);


	template<class T>Archive &operator<<(T &val);
	template<class T>Archive &operator>>(T &val);
	//template<class T>Archive &operator<<(T &val);
	template<>Archive &operator>>(std::string &val);

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


template<class T> Archive &Archive::operator<<(T &val) {
	fs->operator<<<T>(val);
	return *this;
}
template<class T> Archive &Archive::operator>>(T &val) {
	fs->operator>><T>(val);
	return *this;
}
template<> Archive &Archive::operator>><std::string>(std::string &val) {
	val.assign(std::istreambuf_iterator<char>(*fs), std::istreambuf_iterator<char>());
	return *this;
}

#endif // !_archive