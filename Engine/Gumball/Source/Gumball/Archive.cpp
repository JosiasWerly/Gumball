#include "Archive.hpp"

#include <filesystem>

string FilePath::path() const {
	return rpath;
}
string FilePath::name() const {
	const string fName = std::filesystem::path(rpath).filename().string();
	return fName.substr(0, fName.find_last_of(".")); 
}
string FilePath::extension() const {
	return std::filesystem::path(rpath).extension().string().substr(1);
}
u64 FilePath::hash() const {
	return std::hash<string>{}(path());
}
bool FilePath::exists() const {
	return std::filesystem::exists(rpath);
}

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
		this->filePath = FilePath(filePath);
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
Archive &Archive::operator=(const string filePath) {
	open(filePath);
	return *this;
}
bool Archive::operator==(const Archive &other) const {
	return (fs && fs == other.fs);
}