#include "Archive.hpp"


Archive::Archive(string filePath) {
	open(filePath);
}
Archive::Archive(const char *filePath) {
	open(filePath);
}
Archive::Archive(const Archive &other) {
	fs = other.fs;
}
Archive::~Archive() {
	close();
}
void Archive::open(string filePath) {
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
void Archive::writeLine(string strg) {
	*fs << strg + '\n';
}
Inline void Archive::close() {
	if (fs) {
		fs->close();
		delete fs;
		fs = nullptr;
		fsPath = "";
	}
}
bool Archive::getLine(string &str) {
	return (bool)std::getline(*fs, str);
}

template<class T>Archive &Archive::operator<<(T &val) {
	fs.operator<<<T>(val);
	return *this;
}
template<class T>Archive &Archive::operator>>(T &val) {
	fs.operator>><T>(val);
	return *this;
}

Archive &Archive::operator=(const string filePath) {
	open(filePath);
	return *this;
}
bool Archive::operator==(const Archive &other) const {
	return (fs && fs == other.fs);
}