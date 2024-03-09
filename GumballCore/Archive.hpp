#pragma once
#ifndef _archive
#define _archive

#include <string>
#include <fstream>
using namespace std;

class Archive {
	using Long = long long;
	fstream *fs = nullptr;
	string fsPath = "";
public:
	Archive(string filePath);
	Archive(const char *filePath);
	Archive(const Archive &other);
	Archive() = default;
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
	Inline const string &filePath() { return fsPath; }
};

#endif // !_archive