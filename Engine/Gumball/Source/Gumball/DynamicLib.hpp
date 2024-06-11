#pragma once
#ifndef _dynamiclib
#define _dynamiclib

#include <string>

struct HINSTANCE__;
typedef HINSTANCE__ *HINSTANCE;

class DynamicLibrary {
	HINSTANCE instance = nullptr;
	std::string dllPath = "";

	void forceClose();
	long long getFnxAddress(std::string &fName);
public:
	virtual ~DynamicLibrary();
	bool load(std::string dllPath = "");
	void unload();

	HINSTANCE &getHandler() { return instance; };
	bool isLoaded() const { return instance; }
	std::string getPath() const { return dllPath; };

	template<class T> T getFunc(std::string data) {
		return (T)getFnxAddress(data);
	}
};
#endif // !_dynamiclib