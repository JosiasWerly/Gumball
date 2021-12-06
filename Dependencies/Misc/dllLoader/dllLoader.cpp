#include "dllLoader.hpp"

#pragma warning( push )
#pragma warning( disable : 4244 )
void DynamicLibrary::setup(string filePath, string name) {
	this->filePath = filePath;
	this->name = name;
}
bool DynamicLibrary::load() {
	auto WSTRtoSTR = [](wstring& from)->string {
		string out;
		for (size_t i = 0; i < from.length(); i++)
			out += from.at(i);
		return out;
	};
	auto STRtoWSTR = [](string& from)->wstring {
		wstring out;
		for (size_t i = 0; i < from.length(); i++)
			out += from.at(i);
		return out;
	};

	wstring wFullPath = STRtoWSTR(filePath);
	instance = LoadLibrary((LPCWSTR)wFullPath.c_str());
	if (!instance)
		return false;
	return true;
}
void DynamicLibrary::free() {
	if(instance)
		FreeLibrary((HINSTANCE)instance);
}
bool DynamicLibrary::reload() {
	free();
	return load();
}
//template<class T> T DynamicLibrary::getFunc(string data) {
//	T p = nullptr;
//	p = (T)GetProcAddress(instance, (LPCSTR)data.c_str());
//	return p;
//}
#pragma warning( pop )