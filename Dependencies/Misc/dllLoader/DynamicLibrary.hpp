#pragma once
#ifndef _dllLoader
#define _dllLoader

#include <string>
#include <tchar.h>
#include <thread>
using namespace std;


struct HINSTANCE__;
typedef HINSTANCE__ *HINSTANCE;

class DynamicLibrary {
	HINSTANCE instance;
	string dllPath;

	void forceClose();
	long long getFnxAddress(string &fName);
public:
	virtual ~DynamicLibrary();
	bool load(string dllPath = "");
	void unload();

	bool isLoaded() { return instance; }
	string getPath() { return dllPath; };
	HINSTANCE &getHandler() { return instance; };

	template<class T> T getFunc(string data) {
		return (T)getFnxAddress(data);
	}
};
#endif // !_dllLoader
