#include "DynamicLibrary.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//#define _HAS_STD_BYTE 0
#include <libloaderapi.h>

void DynamicLibrary::forceClose() {
	FreeLibraryAndExitThread(instance, DWORD(0));
}
DynamicLibrary::~DynamicLibrary() {
	if (instance)
		unload();
}
bool DynamicLibrary::load(string dllPath) {
	if (instance)
		unload();
	if (dllPath != "")
		this->dllPath = dllPath;
	auto STRtoWSTR = [](string &from)->wstring {
		wstring out;
		for (size_t i = 0; i < from.length(); i++)
			out += from.at(i);
		return out;
	};
	wstring wFullPath = STRtoWSTR(dllPath);
	instance = LoadLibrary((LPCWSTR)wFullPath.c_str());
	if (!instance)
		return false;
	return true;
}
void DynamicLibrary::unload() {
	std::thread th(&DynamicLibrary::forceClose, this);
	th.join();
	instance = nullptr;
}
long long DynamicLibrary::getFnxAddress(string &fName) {
	return (long long)GetProcAddress(instance, (LPCSTR)fName.c_str());
}