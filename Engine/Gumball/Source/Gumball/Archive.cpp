#include "Archive.hpp"

#include <filesystem>

namespace Path {
	string fileName(string path) {
		const string fName = std::filesystem::path(path).filename().string();
		return fName.substr(0, fName.find_last_of("."));
	}
	string extention(string path) {
		return std::filesystem::path(path).extension().string().substr(1);
	}
};

Archive::Archive(string filePath) {
	open(filePath);
}
Archive::Archive(const char *filePath) {
	open(filePath);
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
		this->filePath = FilePath{ filePath };
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
		filePath = FilePath{ "" };
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