#include "FunctionLibrary.hpp"


#include <iostream>
#include <filesystem>
#include <string>
using namespace std;


namespace Files {
	string getExtOfFilePath(string path) {
		return std::filesystem::path(path).extension().string().substr(1);
	}
	string getNameOfFilePath(string path) {
		string fName = std::filesystem::path(path).filename().string();
		return fName.substr(0, fName.find_last_of("."));
	}


};


/*
string getApplicationPath() {
	return std::filesystem::current_path().string();
}


template<class T> string getClassName(){
	string out = typeid(T).name();
	return out.substr(6, out.size() - 6);
}
*/
