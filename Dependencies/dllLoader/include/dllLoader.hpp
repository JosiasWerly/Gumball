#pragma once
#ifndef _dllLoader
#define _dllLoader

//win only
#pragma once
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <list>
using namespace std;

class DynamicLibrary {
public:
	const string name;
	const HINSTANCE instance;
	DynamicLibrary(string name, const HINSTANCE instance);
	template<class T> T getFunc(string data);
};
class DynamicLibraryManager {
	list<DynamicLibrary*> dynamicLibraries;
public:
	bool load(wstring fullPath, string name);
	void free(string name);
	DynamicLibrary* operator[](string name);
};


//#ifdef expDLL
//#define DLL __declspec(dllexport)
//#else
//#define DLL __declspec(dllimport)
//#endif
#endif // !_dllLoader
