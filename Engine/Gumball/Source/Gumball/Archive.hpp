#pragma once
#ifndef _archive
#define _archive

#include <string>
#include <fstream>
#include "Pointer.hpp"
using namespace std;

namespace Path {
	//TODO: change to constString, and rename this namespace...
	string GENGINE fileName(string path);
	string GENGINE extention(string path);
	bool GENGINE exists(string path);
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

#endif // !_archive