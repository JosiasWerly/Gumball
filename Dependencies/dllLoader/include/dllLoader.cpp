#include "dllLoader.hpp"
DynamicLibrary::DynamicLibrary(string name, const HINSTANCE instance) :
	name(name),
	instance(instance) {
}
template<class T> T DynamicLibrary::getFunc(string data) {
	T p = nullptr;
	p = (T)GetProcAddress(instance, (LPCSTR)data.c_str());
	return p;
}

bool DynamicLibraryManager::load(wstring fullPath, string name) {
	if (this->operator[](name))
		return false;
	HINSTANCE instance = LoadLibrary((LPCWSTR)fullPath.c_str());
	if (!instance)
		return false;
	auto converter = [=](wstring& from)->string {
		string stgName;
		for (size_t i = 0; i < from.length(); i++)
			stgName += from.at(i);
		return stgName;
	};
	dynamicLibraries.push_back(new DynamicLibrary(name, instance));
	return true;
}
void DynamicLibraryManager::free(string name) {
	DynamicLibrary* dll = this->operator[](name);
	if (dll) {
		dynamicLibraries.remove(dll);
		FreeLibrary(dll->instance);
		delete dll;
		dll = nullptr;
	}
}
DynamicLibrary* DynamicLibraryManager::operator[](string name) {
	for (auto& d : dynamicLibraries)
		if (d->name == name)
			return d;
	return nullptr;
}