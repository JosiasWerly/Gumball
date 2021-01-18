#pragma once
#ifndef _dllLoader
#define _dllLoader
#include <list>
#include <iostream>
#include <string>

#define _AMD64_
#include <libloaderapi.h>
#include <tchar.h>
using namespace std;

//just getting wild here with win crazy types for windown.h sdk...
class DynamicLibrary {
	HINSTANCE instance;
	string name, filePath;
public:
	virtual ~DynamicLibrary() {
		if (instance)
			free();
	}
	void setup(string filePath, string name);
	bool load();
	void free();
	bool reload();
	template<class T> T getFunc(string data) {
		T p = nullptr;
		p = (T)GetProcAddress(instance, (LPCSTR)data.c_str());
		return p;
	}

	bool isValid() { return instance; }
	string getName() { return name; };
	string getPath() { return filePath; };
	HINSTANCE getHandler() { return instance; };
};


//class DynamicLibraryManager {
//	
//	list<DynamicLibrary*> dynamicLibraries;
//public:
//	bool reload(string name);
//	bool load(string fullPath, string name);
//	void free(string name);
//	DynamicLibrary* operator[](string name);
//};




//#ifdef expDLL
//#define DLL __declspec(dllexport)
//#else
//#define DLL __declspec(dllimport)
//#endif
#endif // !_dllLoader
