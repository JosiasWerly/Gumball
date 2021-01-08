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

//DynamicLibraryManager dm;
//if (dm.load("C:\\Users\\ADM\\Desktop\\Projects\\Gumball\\Build\\Debug\\x64\\GumballTest\\GumballTest.dll", "fun"))
//cout << "loaded" << endl;
//auto dl = dm["fun"];
//typedef BaseScript* (*CreateBaseScript)(void);
//CreateBaseScript fnx = dl->getFunc<CreateBaseScript>("createBaseScript");
//BaseScript* bs = fnx();
//bs->test();
class DynamicLibrary {
public:
	const HINSTANCE instance;
	const string name, filePath;

	DynamicLibrary(string filePath, string name, HINSTANCE instance);
	template<class T> T getFunc(string data){
		T p = nullptr;
		p = (T)GetProcAddress(instance, (LPCSTR)data.c_str());
		return p;
	}
};
class DynamicLibraryManager {
	
	list<DynamicLibrary*> dynamicLibraries;
public:
	bool reload(string name);
	bool load(string fullPath, string name);
	void free(string name);
	DynamicLibrary* operator[](string name);
};




//#ifdef expDLL
//#define DLL __declspec(dllexport)
//#else
//#define DLL __declspec(dllimport)
//#endif
#endif // !_dllLoader
