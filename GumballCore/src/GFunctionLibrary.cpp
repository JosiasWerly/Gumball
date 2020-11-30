#include "GFunctionLibrary.hpp"

string getExtOfFilePath(string path) {
	return std::filesystem::path(path).extension().string().substr(1);
}
string getNameOfFilePath(string path) {
	string fName = std::filesystem::path(path).filename().string();
	return fName.substr(0, fName.find_last_of("."));
}
