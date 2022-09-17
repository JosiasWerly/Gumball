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
	Archive() {}
	Archive(string filePath) {
		open(filePath);
	}
	Archive(const char *filePath) {
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
	Inline const string &filePath() { return fsPath; }

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

#endif // !_archive


